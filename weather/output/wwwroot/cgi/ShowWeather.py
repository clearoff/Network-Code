#!/usr/local/bin/python2.7
#-*- coding:utf-8 -*-

import redis
import smtplib
from email.mime.text import MIMEText

to_list=['597995302@qq.com','1275068381@qq.com']
mail_host="smtp.163.com"
mail_user="15353359226"
mail_pass="s960324"
mail_postfix="163.com"

def send_mail(to_list,sub,content):
	me="hello"+"<"+mail_user+"@"+mail_postfix+">"
	msg = MIMEText(content,_subtype='plain')
	msg['Subject']=sub
	msg['From'] = me
	msg['To'] = ";".join(to_list)
	try:
		server = smtplib.SMTP()
		server.connect(mail_host)
		server.login(mail_user,mail_pass)
		server.sendmail(me,to_list,msg.as_string())
		server.close()
		return True
	except Exception, e:
		print str(e)
		print False

pool = redis.ConnectionPool(host='127.0.0.1',port=6379,db=1)

res = redis.Redis(connection_pool=pool)

count =int(res.hget("weather","days"))

day =""
for i in range(0,count):
	day+= res.hget("weather",'date-'+str(i))
send_mail(to_list,"weather",str(day))

