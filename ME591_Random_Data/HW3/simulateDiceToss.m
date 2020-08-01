%% Data preparation
function [diceValues, sumOfDice] = simulateDiceToss(m)
    diceValues = zeros(1,m);
    sumOfDice = 0;
    for dice = 1:m
        prob = rand;
        if prob <= 1/6
            diceValues(dice) = 1;  
        elseif prob <= 2/6
            diceValues(dice) = 2;
        elseif prob <= 3/6
            diceValues(dice) = 3;
        elseif prob <= 4/6
            diceValues(dice) = 4;
        elseif prob <= 5/6
            diceValues(dice) = 5;
        elseif prob <= 1
            diceValues(dice) = 6;
        end
        sumOfDice = sumOfDice + diceValues(dice);
    end  
end
