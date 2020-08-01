// ant 1 = NB // cons 1 = NB
// ant 2 = Z0 // cons 2 = NM
// ant 3 = PM // cons 3 = Z0
// ant 4 = PB // cons 4 = PM
              // cons 5 = PB

int rules[8][4]= { // rules[no. of rules][no. of I/Os]={ {ant_no, ant_no, cons_no, cons_no}, {,,,}, ........,{,,,}}
    {4,2,5,5},
    {3,2,5,5},
    {2,2,3,3},
    {4,1,5,5},
    {2,1,2,2},
    {2,3,4,4},
    {3,3,3,4},
    {1,2,2,2},
};

/* Rule base that gives a better step response, but output goes crazy when noise is applied

    {4,2,5,5},
    {3,2,5,5},
    {2,2,3,3},
    {4,1,4,4},
    {3,1,3,3},
    {1,1,5,5},
    {1,2,3,3},
    {1,3,2,4},
    {1,4,4,5},
    {2,1,2,2},*/