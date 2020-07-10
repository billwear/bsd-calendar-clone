#!/usr/bin/python3

import datetime
import sys

month1 = [0, 31, 61, 92, 122, 153, 184, 214, 245, 275, 306, 337]
month1s = [0, 31, 61, 92, 122, 153, 184, 214, 235, 265, 296, 327]
jiswitch = [1582, 7, 3]

def ndaysji( year, month, day ):
    return(day + month1[month] + year*365 + year/4)

def idt2date( dt, year, month, day):
    ryear = year - int(year)
    rday = ryear * 365
    print ("ryear " + str(ryear) + " and rday " + str(rday))
    nday = day + 1
    if( month < 10 ):
        nmonth = month + 3
        nyear = year
    else:
        nmonth = month - 9
        nyear = year + 1

    if( nmonth < 1):
        return 0
    else:
        print(str(nyear )+ " " +  str(nmonth) + " " + str(nday))
        dt = datetime.datetime( int(nyear), int(nmonth), int(nday) )
        return( dt )
        
# compute julian date from num of days since march 1 of year 0
def jdate( ndays ):
    idt = datetime.datetime.now()         # internal date representation
    dt = datetime.datetime.now()

    # compute the year by starting with an approximation not smaller
    # than the answer and using linear search for the greatest year
    # which does not begin after ndays
    #
    year = ndays/365
    month = 0
    day = 0
    r = ndaysji(year, month, day)
    while(r > ndays):
        year = year - 1
        r = ndaysji(year, month, day)

    # set r to the days left in the year and compute the month by
    # linear search as the largest month that does not begin after
    # r days
    #
    r = ndays - r
    month = 11
    while( month1[month] > r ):
        month = month - 1

    # compute days left in the month
    day = int( r - month1[month])

    # return external representation of the date
    return idt2date( dt, year, month, day )

date = jdate( 737000 )
print( date )

