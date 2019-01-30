# -*- coding:utf-8 -*-

import sqlite3


d=sqlite3.connect("radar_vul.db")

cursor = d.execute("SELECT id,code from radar_pocs")

for row in cursor:
    print ('loading:'+str(row[0])+'.pyc\n')
    print ("demo:" +str(row[1][:50])+'\n')
    fs = open('output/'+str(row[0])+'.pyc','wb')
    fs.write(row[1])
    fs.close()


print 'finished'
