#!/usr/bin/python3

import datetime

month1 = [0, 31, 61, 92, 122, 153, 184, 214, 245, 275, 306, 337]
month1s = [0, 31, 61, 92, 122, 153, 184, 214, 235, 265, 296, 327]
jiswitch = [1582, 7, 3]

def ndaysji( date ):
    return(date.day + month1[date.month] + date.year*365 + date.year/4)

def idt2date( dt, idt ):
    dt = dt.replace(day = idt.day + 1)
    if( idt.month < 10 ):
        dt = dt.replace(month = idt.month + 3)
        dt = dt.replace(year = idt.year)
    else:
        dt = dt.replace(month = idt.month - 9)
        dt = dt.replace(year = idt.year + 1)

    if( dt.month < 1):
        return None
    else:
        return( dt )
        
# compute julian date from num of days since march 1 of year 0
def jdate( ndays, dt ):
    idt = datetime()          # internal date representation

    # compute the year by starting with an approximation not smaller
    # than the answer and using linear search for the greatest year
    # which does not begin after ndays
    #
    idt.year = ndays / 365
    idt.month = 0
    idt.day = 0
    r = ndaysji(idt)
    while(r > ndays):
        idt.year -= 1
        r = ndaysji(idt)

    # set r to the days left in the year and compute the month by
    # linear search as the largest month that does not begin after
    # r days
    #
    r = ndays -r
    idt.month = 11
    while( month1[idt.month] > r ):
        idt.month -= 1

    # compute days left in the month
    idt.day = r - month1[idt.month]

    # return external representation of the date
    return idt2date( dt, idt )

dt = datetime.datetime(2019, )
x = datetime.datetime.now()
y = ndaysji(x)
date = idt2date( dt, x )

print ("date is " + str(x))
print ("ndays is " + str(y))

