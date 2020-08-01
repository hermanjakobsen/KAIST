#!/usr/bin/python3

from __future__ import print_function

from twisted.internet import reactor
from twisted.internet.defer import inlineCallbacks

from autobahn.wamp.serializer import MsgPackSerializer
from autobahn.wamp.types import ComponentConfig
from autobahn.twisted.wamp import ApplicationSession, ApplicationRunner

import argparse
import random
import math
import sys

import base64
import numpy as np

import helper

####### My imports #######
import fuzzy

#########################


# reset_reason
NONE = 0
GAME_START = 1
SCORE_MYTEAM = 2
SCORE_OPPONENT = 3
GAME_END = 4
DEADLOCK = 5
GOALKICK = 6
CORNERKICK = 7
PENALTYKICK = 8
HALFTIME = 9
EPISODE_END = 10

# game_state
STATE_DEFAULT = 0
STATE_KICKOFF = 1
STATE_GOALKICK = 2
STATE_CORNERKICK = 3
STATE_PENALTYKICK = 4

# coordinates
MY_TEAM = 0
OP_TEAM = 1
BALL = 2
X = 0
Y = 1
TH = 2
ACTIVE = 3
TOUCH = 4


class Received_Image(object):
    def __init__(self, resolution, colorChannels):
        self.resolution = resolution
        self.colorChannels = colorChannels
        # need to initialize the matrix at timestep 0
        self.ImageBuffer = np.zeros((resolution[1], resolution[0], colorChannels))  # rows, columns, colorchannels

    def update_image(self, received_parts):
        self.received_parts = received_parts
        for i in range(0, len(received_parts)):
            dec_msg = base64.b64decode(self.received_parts[i].b64, '-_')  # decode the base64 message
            np_msg = np.fromstring(dec_msg, dtype=np.uint8)  # convert byte array to numpy array
            reshaped_msg = np_msg.reshape((self.received_parts[i].height, self.received_parts[i].width, 3))
            for j in range(0, self.received_parts[i].height):  # y axis
                for k in range(0, self.received_parts[i].width):  # x axis
                    self.ImageBuffer[j + self.received_parts[i].y, k + self.received_parts[i].x, 0] = reshaped_msg[
                        j, k, 0]  # blue channel
                    self.ImageBuffer[j + self.received_parts[i].y, k + self.received_parts[i].x, 1] = reshaped_msg[
                        j, k, 1]  # green channel
                    self.ImageBuffer[j + self.received_parts[i].y, k + self.received_parts[i].x, 2] = reshaped_msg[
                        j, k, 2]  # red channel


class SubImage(object):
    def __init__(self, x, y, width, height, b64):
        self.x = x
        self.y = y
        self.width = width
        self.height = height
        self.b64 = b64


class Frame(object):
    def __init__(self):
        self.time = None
        self.score = None
        self.reset_reason = None
        self.game_state = None
        self.subimages = None
        self.coordinates = None
        self.half_passed = None


class Component(ApplicationSession):
    """
    AI Base + Rule Based Algorithm
    """

    def __init__(self, config):
        ApplicationSession.__init__(self, config)

    def printConsole(self, message):
        print(message)
        sys.__stdout__.flush()

    def onConnect(self):
        self.join(self.config.realm)

    @inlineCallbacks
    def onJoin(self, details):

        ##############################################################################
        def init_variables(self, info):
            # Here you have the information of the game (virtual init() in random_walk.cpp)
            # List: game_time, number_of_robots
            #       field, goal, penalty_area, goal_area, resolution Dimension: [x, y]
            #       ball_radius, ball_mass,
            #       robot_size, robot_height, axle_length, robot_body_mass, ID: [0, 1, 2, 3, 4]
            #       wheel_radius, wheel_mass, ID: [0, 1, 2, 3, 4]
            #       max_linear_velocity, max_torque, codewords, ID: [0, 1, 2, 3, 4]
            self.game_time = info['game_time']
            self.number_of_robots = info['number_of_robots']

            self.field = info['field']
            self.goal = info['goal']
            self.penalty_area = info['penalty_area']
            # self.goal_area = info['goal_area']
            self.resolution = info['resolution']

            self.ball_radius = info['ball_radius']
            # self.ball_mass = info['ball_mass']

            self.robot_size = info['robot_size']
            # self.robot_height = info['robot_height']
            # self.axle_length = info['axle_length']
            # self.robot_body_mass = info['robot_body_mass']

            # self.wheel_radius = info['wheel_radius']
            # self.wheel_mass = info['wheel_mass']

            self.max_linear_velocity = info['max_linear_velocity']
            # self.max_torque = info['max_torque']
            # self.codewords = info['codewords']

            self.colorChannels = 3
            self.end_of_frame = False
            self.image = Received_Image(self.resolution, self.colorChannels)
            self.cur_posture = []
            self.cur_ball = []
            self.prev_posture = []
            self.prev_ball = []
            self.previous_frame = Frame()
            self.received_frame = Frame()

            self.cur_count = 0
            self.end_count = 0
            self.prev_sender = None
            self.sender = None
            self.touch = [False,False,False,False,False]
            self.prev_receiver = None
            self.receiver = None
            self.def_idx = 0
            self.atk_idx = 0
            self.closest_order = []
            self.player_state = [None,None,None,None,None]

            self.bugFix = 0
            self.passingSeq = None
            self.xDesired = 0
            self.yDesired = 0

            self.wheels = [0 for _ in range(10)]
            return

        ##############################################################################

        try:
            info = yield self.call(u'aiwc.get_info', args.key)
        except Exception as e:
            self.printConsole("Error: {}".format(e))
        else:
            try:
                self.sub = yield self.subscribe(self.on_event, args.key)
            except Exception as e2:
                self.printConsole("Error: {}".format(e2))

        init_variables(self, info)

        try:
            yield self.call(u'aiwc.ready', args.key)
        except Exception as e:
            self.printConsole("Error: {}".format(e))
        else:
            self.printConsole("I am ready for the game!")

    # set the left and right wheel velocities of robot with id 'id'
    # 'max_velocity' scales the velocities up to the point where at least one of wheel is operating at max velocity
    def set_wheel_velocity(self, id, left_wheel, right_wheel):
        self.wheels[2 * id] = left_wheel
        self.wheels[2 * id + 1] = right_wheel

    # let the robot with id 'id' move to a target position (x, y)
    # the trajectory to reach the target position is determined by several different parameters
    def set_target_position(self, id, x, y):
        vl, vr, _, _, = fuzzy.FLC(xRef=x, yRef=y, x=self.cur_posture[id][X], y=self.cur_posture[id][Y], th=self.cur_posture[id][TH], mode='position')
        self.set_wheel_velocity(id, vl, vr)

    # copy coordinates from frames to different variables just for convenience
    def get_coord(self):
        self.cur_ball = self.received_frame.coordinates[BALL]
        self.cur_posture = self.received_frame.coordinates[MY_TEAM]
        self.cur_posture_op = self.received_frame.coordinates[OP_TEAM]
        self.prev_ball = self.previous_frame.coordinates[BALL]
        self.prev_posture = self.previous_frame.coordinates[MY_TEAM]
        self.prev_posture_op = self.previous_frame.coordinates[OP_TEAM]

    # find a defender and a forward closest to the ball
    def find_closest_robot(self):
        # find the closest defender
        min_idx = 0
        min_dist = 9999.99
        def_dist = 9999.99

        all_dist = []

        for i in [1, 2]:
            measured_dist = helper.dist(self.cur_ball[X], self.cur_posture[i][X], self.cur_ball[Y],
                                                self.cur_posture[i][Y])
            all_dist.append(measured_dist)
            if (measured_dist < min_dist):
                min_dist = measured_dist
                def_dist = min_dist
                min_idx = i

        self.def_idx = min_idx

        # find the closest forward
        min_idx = 0
        min_dist = 9999.99
        atk_dist = 9999.99

        for i in [3, 4]:
            measured_dist = helper.dist(self.cur_ball[X], self.cur_posture[i][X], self.cur_ball[Y],
                                                self.cur_posture[i][Y])
            all_dist.append(measured_dist)
            if (measured_dist < min_dist):
                min_dist = measured_dist
                atk_dist = min_dist
                min_idx = i

        self.atk_idx = min_idx

        # record the robot closer to the ball between the two too
        self.closest_order = np.argsort(all_dist) + 1

    # predict where the ball will be located after 'steps' steps
    def predict_ball_location(self, steps):
        dx = self.cur_ball[X] - self.prev_ball[X]
        dy = self.cur_ball[Y] - self.prev_ball[Y]
        return [self.cur_ball[X] + steps * dx, self.cur_ball[Y] + steps * dy]

    # let the robot face toward specific direction
    def face_specific_position(self, id, x, y):
        dx = x - self.cur_posture[id][X]
        dy = y - self.cur_posture[id][Y]

        desired_th = (math.pi / 2) if (dx == 0 and dy == 0) else math.atan2(dy, dx)

        self.angle(id, desired_th)

    # returns the angle toward a specific position from current robot posture
    def direction_angle(self, id, x, y):
        dx = x - self.cur_posture[id][X]
        dy = y - self.cur_posture[id][Y]

        return ((math.pi / 2) if (dx == 0 and dy == 0) else math.atan2(dy, dx))

    # turn to face 'desired_th' direction
    def angle(self, id, desired_th):
        vl, vr, _, _ = fuzzy.FLC(thRef=desired_th, th=self.cur_posture[id][TH], mode='angle')

        self.set_wheel_velocity(id, vl, vr)

    # checks if a certain position is inside the penalty area of 'team'
    def in_penalty_area(self, obj, team):
        if (abs(obj[Y]) > self.penalty_area[Y] / 2):
            return False

        if (team == MY_TEAM):
            return (obj[X] < -self.field[X] / 2 + self.penalty_area[X])
        else:
            return (obj[X] > self.field[X] / 2 - self.penalty_area[X])

    # check if the ball is coming toward the robot
    def ball_coming_toward_robot(self, id):
        x_dir = abs(self.cur_posture[id][X] - self.prev_ball[X]) > abs(self.cur_posture[id][X] - self.cur_ball[X])
        y_dir = abs(self.cur_posture[id][Y] - self.prev_ball[Y]) > abs(self.cur_posture[id][Y] - self.cur_ball[Y])

        # ball is coming closer
        if (x_dir and y_dir):
            return True
        else:
            return False

    # check if sender/receiver pair should be reset
    def reset_condition(self) :
        # if the time is over, setting is reset
        if (self.end_count > 0 and self.end_count - self.cur_count < 0) :
            return True

        # if there is no sender and receiver is not in shoot chance, setting is cleared
        if not self.sender is None :
            if not self.shoot_chance(self.sender) :
                return True
        return False

    # check if a sender can be selected
    def set_sender_condition(self) :
        for i in range(1,5) :
            # if this robot is near the ball, it will be a sender candidate
            dist = helper.dist(self.cur_posture[i][X], self.cur_ball[X], self.cur_posture[i][Y], self.cur_ball[Y])
            if dist < 0.5 and self.cur_posture[i][ACTIVE]: return True
        return False

    # check if a receiver should be selected
    def set_receiver_condition(self) :
        # if a sender exists, any other robots can be receiver candidates
        if self.sender != None and self.receiver == None: return True
        return False

    # select a sender
    def set_sender(self, _player_list):
        distance_list = []
        for sender in _player_list :
            predict_ball = self.predict_ball_location(3)
            ball_distance = helper.dist(predict_ball[X], self.cur_posture[sender][X], predict_ball[Y], self.cur_posture[sender][Y])
            distance_list.append(ball_distance)

        # if the distance between ball and sender is less than 1, choose the closest robot as the sender
        if min(distance_list) < 1.0 :
            return distance_list.index(min(distance_list)) + 1

        # otherwise, there is no sender
        return None

    # select a receiver
    def set_receiver(self, _player_list):
        receiver_op_dist_list = []
        for receiver in _player_list :
            temp_receiver_op_dist_list = []
            # the sender is not a receiver candidate
            if receiver == self.sender :
                receiver_op_dist_list.append(999)
                continue

            # the distance between the robot and opponents
            for op in range(1, 5) : #[1,2,3,4]
                op_distance = helper.dist(self.cur_posture[receiver][X], self.cur_posture_op[op][X], self.cur_posture[receiver][Y], self.cur_posture_op[op][Y])
                temp_receiver_op_dist_list.append(op_distance)

            # save the shortest distance between this robot and one of opponents
            receiver_op_dist_list.append(min(temp_receiver_op_dist_list))

        receiver_ball_list = []
        for r in receiver_op_dist_list :
            # if the minimum distance between player and opponent's player is less than 0.5, this robot cannot be receiver
            if r < 0.5 or r == 999:
                receiver_ball_list.append(999)
                continue
            id = receiver_op_dist_list.index(r) + 1
            receiver_ball_distance = helper.dist(self.cur_ball[X], self.cur_posture[id][X], self.cur_ball[Y], self.cur_posture[id][Y])
            receiver_ball_list.append(receiver_ball_distance)

        if min(receiver_ball_list) < 999 :
            min_id = receiver_ball_list.index(min(receiver_ball_list)) + 1
            return min_id
        return None

    def pass_ball(self):
        if self.prev_sender == self.receiver or self.prev_receiver == self.sender :# and not None in [self.prev_sender, self.prev_receiver, self.sender, self.receiver] :
            self.sender = self.prev_sender
            self.receiver = self.prev_receiver

        self.receive_ball()
        self.send_ball()

        self.prev_sender = self.sender
        self.prev_receiver = self.receiver

    def send_ball(self) :
        if self.sender == None :
            return

        goal_dist = helper.dist(4.0, self.cur_posture[self.sender][X], 0, self.cur_posture[self.sender][Y])
        # if the sender has a shoot chance, it tries to shoot
        if self.shoot_chance(self.sender) :
            if goal_dist > 0.3 * self.field[X] / 2:
                self.actions(self.sender, 'dribble',refine=True)
                return
            else :
                self.actions(self.sender, 'kick')
                return

        # if the receiver exists, get the distance between the sender and the receiver
        sender_receiver_dist = None
        if not self.receiver == None :
            sender_receiver_dist = helper.dist(self.cur_posture[self.sender][X], self.cur_posture[self.receiver][X],self.cur_posture[self.sender][Y], self.cur_posture[self.receiver][Y])

        # if the sender is close to the receiver, the sender kicks the ball
        if not sender_receiver_dist == None :
            if sender_receiver_dist < 0.3 and not self.cur_posture[self.receiver][TOUCH]:
                self.actions(self.sender, 'kick')
                return

        ift, theta_diff = self.is_facing_target(self.sender, self.cur_ball[X], self.cur_ball[Y])
        if not ift :
            # after the sender kicks, it stops
            if theta_diff > math.pi * 3/4 :
                self.actions(self.sender, None)
                return
            else :
                self.actions(self.sender, 'follow',refine=True)
                return

        # if the ball is in front of the sender and sender is moving backward
        if self.cur_posture[self.sender][X] < - 0.8 * self.field[X] / 2 :
            if self.cur_posture[self.sender][X] - self.prev_posture[self.sender][X] < 0 :
                self.actions(self.sender, 'backward')

        self.actions(self.sender, 'dribble',refine=True)
        return

    def receive_ball(self) :
        # if receiver does not exist, do nothing
        if self.receiver == None :
            return

        goal_dist = helper.dist(4.0, self.cur_posture[self.receiver][X], 0, self.cur_posture[self.receiver][Y])
        # if sender is in shoot chance, receiver does nothing(reset)
        if self.shoot_chance(self.sender) :
            self.actions(self.receiver,None)
            return
        # if receiver is in shoot chance, receiver try to shoot
        if self.shoot_chance(self.receiver) :
            if goal_dist > 0.3 * self.field[X] / 2:
                self.actions(self.receiver, 'dribble',refine=True)
                return
            else :
                self.actions(self.receiver, 'kick')
                return

        # if sender exists
        if not self.sender == None :
            s2risFace, _ = self.is_facing_target(self.sender, self.cur_posture[self.receiver][X], self.cur_posture[self.receiver][Y],4)
            r2sisFace, _ = self.is_facing_target(self.receiver, self.cur_posture[self.sender][X], self.cur_posture[self.sender][Y],4)
            # if sender and receiver directs each other
            if s2risFace and r2sisFace :
                if self.cur_posture[self.receiver][TH] > 0 or self.cur_posture[self.receiver][TH] < -3 :
                    self.actions(self.receiver,'follow', [self.prev_posture[self.receiver][X], self.prev_posture[self.receiver][Y] - 0.5 * self.field[Y]])
                    return
                self.actions(self.receiver, 'follow',[self.prev_posture[self.receiver][X], self.prev_posture[self.receiver][Y] + 0.5 * self.field[Y]])
                return

        r_point = self.cur_ball
        # if sender exists
        if not self.sender == None:
            r_point = self.receive_position()
        receiver_ball_dist = helper.dist(self.cur_ball[X], self.cur_posture[self.receiver][X], self.cur_ball[Y],self.cur_posture[self.receiver][Y])
        # if ball is close to receiver
        if receiver_ball_dist > 0.3 * self.field[X] / 2 :
            self.actions(self.receiver, 'follow', [r_point[X], r_point[Y]],refine=True)
            return

        r2bisFace, _ = self.is_facing_target(self.receiver, self.cur_ball[X], self.cur_ball[Y], 4)
        if not r2bisFace :
            self.actions(self.receiver, 'follow',refine=True)
            return
        # if receiver is moving to our goal area
        if self.cur_posture[self.receiver][X] < - 0.8 * self.field[X] / 2 :
            if self.cur_posture[self.receiver][X] - self.prev_posture[self.receiver][X] < 0 :
                self.actions(self.receiver, 'backward')

        self.actions(self.receiver, 'dribble')
        return

    # let robot with id 'id' execute an action directed by 'mode'
    def actions(self, id, mode = None, target_pts = None, params = None, refine = False) :
        if id == None :
            return

        # if the player state is set to 'stop', force the mode to be 'stop'
        if self.player_state[id] == 'stop' :
            mode = 'stop'

        if mode == None :
            # reset all robot status
            if self.sender == id :
                self.sender = None
                self.touch = [False, False, False, False, False]
            if self.receiver == id :
                self.receiver = None
            self.player_state[id] = None
            return
        if mode == 'follow' :
            # let the robot follow the ball
            if target_pts == None :
                target_pts = self.predict_ball_location(3)
            if params == None :
                params = [1.0, 3.0, 0.6, False]
            if refine :
                self.set_pos_parameters(id, target_pts, params)
            self.set_target_position(id, target_pts[X], target_pts[Y])
            self.player_state[id] = 'follow'
            return
        if mode == 'dribble' :
            # let the robot follow the ball but at a faster speed
            if target_pts == None :
                target_pts = self.cur_ball
            if params == None :
                params = [1.4, 5.0, 0.8, False]
            if refine :
                self.set_pos_parameters(id, target_pts, params)
            self.set_target_position(id, target_pts[X], target_pts[Y])
            self.player_state[id] = 'dribble'
            return
        if mode == 'kick' :
            # kick the ball
            if target_pts == None :
                target_pts = self.cur_ball
            if params == None :
                params = [1.4, 5.0, 0.8, True]
            if self.end_count == 0 and not self.touch[id] :
                self.end_count = self.cur_count + 10 # 0.05 * cnt seconds
            self.player_state[id] = 'kick'
            if self.touch[id] :
                self.player_state[id] = 'stop'
            if not self.touch[id] :
                self.touch[id] = self.cur_posture[id][TOUCH]
            if self.player_state[id] == 'stop' :
                params = [0.0, 0.0, 0.0, False]
            self.set_target_position(id, target_pts[X], target_pts[Y])
            return
        if mode == 'stop' :
            # stop while counter is on
            if params == None :
                params = [0.0, 0.0, False]
            self.set_wheel_velocity(id, params[0], params[1], params[2])
            if self.end_count == 0 :
                self.end_count = self.cur_count + 5 # 0.05 * cnt seconds
            self.player_state[id] = 'stop'
            if self.end_count - 1 == self.cur_count :
                self.player_state[id] = None
            return
        if mode == 'backward' :
            # retreat from the current position
            if target_pts == None :
                target_pts = [self.cur_posture[id][X] + 0.2, self.cur_posture[id][Y]]
            if params == None :
                params = [1.4, 5.0, 0.8, False]
            if refine :
                self.set_pos_parameters(id, target_pts, params)
            self.set_target_position(id, target_pts[X], target_pts[Y])
            self.player_state[id] = 'backward'
            return
        if mode == 'position' :
            # go toward target position
            self.set_target_position(id, target_pts[X], target_pts[Y])
            return

    def is_facing_target(self, id, x, y):
        desiredTh = self.direction_angle(id, x, y)
        thError = helper.trim_radian(desiredTh - self.cur_posture[id][TH])
        return (abs(thError) < 0.2)

    def receive_position(self):
        step = 5
        ball_receiver_dist = helper.dist(self.cur_ball[X], self.cur_posture[self.receiver][X], self.cur_ball[Y],
                                             self.cur_posture[self.receiver][Y])
        prev_ball_receiver_dist = helper.dist(self.prev_ball[X], self.prev_posture[self.receiver][X],
                                                  self.prev_ball[Y], self.prev_posture[self.receiver][Y])

        diff_dist = prev_ball_receiver_dist - ball_receiver_dist
        if diff_dist > 0:
            step = ball_receiver_dist # diff_dist

        step = min(step, 15)

        predict_pass_point = self.predict_ball_location(step)

        ball_goal_dist = helper.dist(self.cur_ball[X], self.field[X] / 2, self.cur_ball[Y], 0)
        prev_ball_goal_dist = helper.dist(self.prev_ball[X], self.field[X] / 2, self.prev_ball[Y], 0)
        if ball_goal_dist > prev_ball_goal_dist:
            predict_pass_point[X] = predict_pass_point[X] - 0.15

        return predict_pass_point
        

    ############ Benchmark functions #############   
     
    def move_other_robots_away(self, id):
        for i in range(self.number_of_robots):
            if (i != id):
                self.set_target_position(i, -3, -2)


    def move_into_shooting_position(self, id):
        # if the robot is blocking the ball's path toward opponent side. Go around ball instead (Used for kick off)
        if (self.cur_ball[X] > -0.3 * self.field[X] / 2 and self.cur_ball[X] < 0.3 * self.field[X] / 2 and self.cur_posture[id][X] > self.cur_ball[X] + 0.1 and abs(self.cur_posture[id][Y] - self.cur_ball[Y]) < 0.3):
            if (self.cur_ball[Y] < 0):
                self.set_target_position(id, self.cur_posture[id][X] - 0.25, self.cur_ball[Y] + 1)
            else:
                self.set_target_position(id, self.cur_posture[id][X] - 0.25, self.cur_ball[Y] - 1)
            return

        # Calculate line between goal and ball and later position robot at this line
        centerGoalX = self.field[X] / 2
        centerGoalY = 0
        a = (centerGoalY - self.cur_ball[Y]) / (centerGoalX - self.cur_ball[X])
        y = a*(self.cur_ball[X]-0.5) - a*self.cur_ball[X] + self.cur_ball[Y]

        # Go into a good shooting position
        # Kick-off
        if (self.cur_ball[X] == 0 and self.cur_ball[Y] == 0):
            self.set_target_position(id, self.cur_ball[X] - 0.5, 0.2)

        elif (self.cur_ball[X] > -0.3 * self.field[X] / 2):
            self.set_target_position(id, self.cur_ball[X] - 0.5, y-0.1)

        else:
            self.set_target_position(id, -0.1 * self.field[X] / 2, self.cur_ball[Y])


    # check if the robot with id 'id' has a chance to shoot
    # !!!!!!!!!!!!!! HAS A BUG WHEN THE BALL IS PUT ON THE MIDDLE AFTER A GOAL IS SCORED !!!!!!!!!!

    def in_position(self, id, xTarget, yTarget, tol):
        return (helper.dist(self.xDesired, self.cur_posture[id][X], self.yDesired, self.cur_posture[id][Y]) < tol)
    

    def shoot_chance(self, id):
        centerGoalX = self.field[X] / 2
        centerGoalY = 0
        a = (centerGoalY - self.cur_ball[Y]) / (centerGoalX - self.cur_ball[X])
        yOpt = a*(self.cur_ball[X]-0.5) - a*self.cur_ball[X] + self.cur_ball[Y]
        yReal = a*self.cur_posture[id][X] - a*self.cur_ball[X] + self.cur_ball[Y]

        # Special case for kick off when starting on opposite side
        if (self.cur_ball[X] == 0 and self.cur_ball[Y] == 0 and self.cur_posture[id][X] > -0.49):
            return False

        # Special case for kick off when starting on same side
        #elif (self.cur_ball[X] == 0 and self.cur_ball[Y] == 0 and self.cur_posture[id][Y] < -0.05):
         #   return False 

        # if the robot->ball direction aligns with opponent's goal, the robot can shoot
        if (abs(yOpt- yReal) < 0.05):
            return True
        else:
            return False


    # Benchmark 1 is to score a goal from different positions
    # Should be used with id = 4
    def benchmark1(self, id):
        self.printConsole(self.player_state[id])
        self.move_other_robots_away(id)
        if (self.player_state[id] == None):
            self.end_count = 0
            self.cur_count = 0
            self.bugFix = 0
            self.player_state[id] = 'position'
        
        if (self.player_state[id] == 'position'):
            self.move_into_shooting_position(id)
            self.move_other_robots_away(id)
            self.bugFix += 1
            if self.shoot_chance(id) and self.bugFix > 5:
                self.player_state[id] = 'angle'

        if self.player_state[id] == 'angle':
            self.move_other_robots_away(id)
            self.face_specific_position(id, self.field[X]/2, 0)
            if (self.is_facing_target(id, self.field[X]/2, 0)):
                self.player_state[id] = 'shoot'

        if self.player_state[id] == 'shoot':
            self.move_other_robots_away(id)
            self.set_wheel_velocity(id, self.max_linear_velocity[id], self.max_linear_velocity[id])
            if(self.cur_posture[id][TOUCH]):
                self.player_state[id] = 'stop'

        if self.player_state[id] == 'stop':
            self.move_other_robots_away(id)
            self.set_wheel_velocity(id, 0, 0)

            # Wait some time after shot
            if self.end_count == 0 :
                self.end_count = self.cur_count + 50 # 0.05 * cnt seconds
            self.cur_count += 1
            if self.end_count == self.cur_count :
                self.player_state[id] = None     

    # A little too inaccurate and sketchy behaviour when slopes are steep
    def move_into_passing_position(self, id, xTarget, yTarget):
        
        # if the robot is blocking the ball's path toward target side. Go around ball instead
        if (self.cur_ball[X] > -0.3 * xTarget and self.cur_ball[X] < 0.3 * xTarget and self.cur_posture[id][X] > self.cur_ball[X] + 0.1 and abs(self.cur_posture[id][Y] - self.cur_ball[Y]) < 0.3):
            if (self.cur_ball[Y] < 0):
                self.set_target_position(id, self.cur_posture[id][X] - 0.25, self.cur_ball[Y] + 1)
            else:
                self.set_target_position(id, self.cur_posture[id][X] - 0.25, self.cur_ball[Y] - 1)
            return

        # Calculate line between target and ball and later position robot at this line
        a = (yTarget - self.cur_ball[Y]) / (xTarget - self.cur_ball[X])

        if (self.cur_ball[X] < xTarget):
            x = self.cur_ball[X] - 0.5
            y = a*(self.cur_ball[X] - 0.5) - a*self.cur_ball[X] + self.cur_ball[Y]
            while(y > 2.1): # Trim the y-coordinate if the slope 'a' is too big. Can also use distance between ball and robot as condition
                y = a*x - a*self.cur_ball[X] + self.cur_ball[Y]
                x += x/2
        else:
            x = self.cur_ball[X] + 0.5
            y = a*x - a*self.cur_ball[X] + self.cur_ball[Y]
            while(y < -2.1):
                y = a*x - a*self.cur_ball[X] + self.cur_ball[Y]
                x -= x/2

        self.xDesired = x
        self.yDesired = y

        # Move the robot
        if (self.cur_ball[X] > -0.3 * xTarget):
            self.set_target_position(id, x, y)
        else:
            self.set_target_position(id, x, y)


    def pass_between_robots(self, idPasser=None, idReceiver=None, xTarget=None, yTarget=None):
        if (self.player_state[idPasser] == None):
            self.end_count = 0
            self.cur_count = 0
            self.bugFix = 0     # Used to skip frames due to faulty values at the start
            self.player_state[idPasser] = 'position'
        
        if (self.player_state[idPasser] == 'position'):
            self.set_target_position(idReceiver, xTarget, yTarget)
            self.move_into_passing_position(idPasser, xTarget, yTarget)
            self.bugFix += 1
            if self.in_position(idPasser, xTarget, yTarget, 0.12) and self.bugFix > 4:
                    self.player_state[idPasser] = 'angle'

        if self.player_state[idPasser] == 'angle':
            self.set_target_position(idReceiver, xTarget, yTarget)
            self.face_specific_position(idPasser, xTarget, yTarget)
            if (self.is_facing_target(idPasser, xTarget, yTarget)):
                self.player_state[idPasser] = 'pass'

        if self.player_state[idPasser] == 'pass':
            self.set_target_position(idReceiver, xTarget, yTarget)
            self.set_wheel_velocity(idPasser, self.max_linear_velocity[idPasser]*0.36, self.max_linear_velocity[idPasser]*0.36)
            if(self.cur_posture[idPasser][TOUCH]):
               self.player_state[idPasser] = 'stop'

        if self.player_state[idPasser] == 'stop':
            self.set_target_position(idReceiver, xTarget, yTarget)
            self.set_wheel_velocity(idPasser, 0, 0)

            # Wait some time after pass
            if self.end_count == 0 :
                self.end_count = self.cur_count + 25 # 0.05 * cnt seconds
            self.cur_count += 1
            if self.end_count == self.cur_count:
                self.player_state[idPasser] = None
           

    def pass_received(self, idReceiver):
        return (helper.dist(self.cur_posture[idReceiver][X], self.cur_ball[X], self.cur_posture[idReceiver][Y], self.cur_ball[Y]) < 0.2)

    # Benchmark problem is to pass between robots
    def benchmark2(self):
        self.printConsole(self.passingSeq)
        # Define the different passing points
        p1x = 1.9
        p1y = -2
        p2x = 2.1
        p2y = 1.8
        p3x = -0.8
        p3y = 2.3
        
        if self.passingSeq == None:
            self.passingSeq = 'first'

        if self.passingSeq == 'first':
            idPasser = 4
            idReceiver = 3
            self.pass_between_robots(idPasser, idReceiver, p1x, p1y)
            if self.pass_received(idReceiver):
                self.passingSeq = 'second'

        if self.passingSeq == 'second':
            idPasser = 3
            idReceiver = 2
            self.pass_between_robots(idPasser, idReceiver, p2x, p2y)
            if self.pass_received(idReceiver):
                self.passingSeq = 'third'

        if self.passingSeq == 'third':
            idPasser = 2
            idReceiver = 4
            self.pass_between_robots(idPasser, idReceiver, p3x, p3y)
            if self.pass_received(idReceiver):
                self.printConsole("SKRRT!")


    #####################################

    @inlineCallbacks
    def on_event(self, f):

        @inlineCallbacks
        def set_wheel(self, robot_wheels):
            yield self.call(u'aiwc.set_speed', args.key, robot_wheels)
            return

         # initiate empty frame
        if (self.end_of_frame):
            self.received_frame = Frame()
            self.end_of_frame = False
        received_subimages = []

        if 'time' in f:
            self.received_frame.time = f['time']
        if 'score' in f:
            self.received_frame.score = f['score']
        if 'reset_reason' in f:
            self.received_frame.reset_reason = f['reset_reason']
        if 'subimages' in f:
            self.received_frame.subimages = f['subimages']

            # Uncomment following block to use images.
            # for s in self.received_frame.subimages:
            #     received_subimages.append(SubImage(s['x'],
            #                                        s['y'],
            #                                        s['w'],
            #                                        s['h'],
            #                                        s['base64'].encode('utf8')))
            # self.image.update_image(received_subimages)
        if 'coordinates' in f:
            self.received_frame.coordinates = f['coordinates']
        if 'EOF' in f:
            self.end_of_frame = f['EOF']

        #self.printConsole(self.received_frame.time)
        #self.printConsole(self.received_frame.score)
        #self.printConsole(self.received_frame.reset_reason)
        #self.printConsole(self.end_of_frame)

        
##############################################################################


    ##############################################################################    
    # RUN BENCHMARKS            
            #(virtual update())
            if (self.received_frame.reset_reason != NONE):
                self.previous_frame = self.received_frame
            self.get_coord()
            self.get_coord()

            #p1x = 1.9
            #p1y = -2
            #p2x = 2.1
            #p2y = 1.8
            #p3x = -0.8
            #p3y = 2.3

            #if self.pass_chance(4, p3x, p3y):
            #    self.set_wheel_velocity(4, 0, 0)
            #else:
            #    self.move_into_passing_position(4, p3x, p3y) 
            
            
            self.benchmark1(4)
            #self.benchmark2()

            set_wheel(self, self.wheels)
##############################################################################

            if (self.received_frame.reset_reason == GAME_END):
##############################################################################
                #(virtual finish())
                #save your data
                with open(args.datapath + '/result.txt', 'w') as output:
                    #output.write('yourvariables')
                    output.close()
                #unsubscribe; reset or leave
                yield self.sub.unsubscribe()
                try:
                    yield self.leave()
                except Exception as e:
                    self.printConsole("Error: {}".format(e))
##############################################################################

    def onDisconnect(self):
        if reactor.running:
            reactor.stop()


if __name__ == '__main__':

    try:
        unicode
    except NameError:
        # Define 'unicode' for Python 3
        def unicode(s, *_):
            return s


    def to_unicode(s):
        return unicode(s, "utf-8")


    parser = argparse.ArgumentParser()
    parser.add_argument("server_ip", type=to_unicode)
    parser.add_argument("port", type=to_unicode)
    parser.add_argument("realm", type=to_unicode)
    parser.add_argument("key", type=to_unicode)
    parser.add_argument("datapath", type=to_unicode)

    args = parser.parse_args()

    ai_sv = "rs://" + args.server_ip + ":" + args.port
    ai_realm = args.realm

    # create a Wamp session object
    session = Component(ComponentConfig(ai_realm, {}))

    # initialize the msgpack serializer
    serializer = MsgPackSerializer()

    # use Wamp-over-rawsocket
    runner = ApplicationRunner(ai_sv, ai_realm, serializers=[serializer])

    runner.run(session, auto_reconnect=False)
