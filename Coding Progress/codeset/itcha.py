#coding=utf-8

import itchat

itchat.auto_login()

itchat.send('Hello, filehelper1012', toUserName='filehelper')

#users=itchat.search_friends(NickName="荣丰宇")

users=itchat.get_friends()

username=users[0]['UserName']

print(username)

itchat.send('你好', toUserName=username)
