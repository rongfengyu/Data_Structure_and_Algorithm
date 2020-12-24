#coding=utf-8

import rsa

pubkey, privkey = rsa.newkeys(1024)

# 公钥
pub = pubkey.save_pkcs1()
with open('public.pem', 'wb') as w_pub:
  w_pub.write(pub)
# 私钥
pri = privkey.save_pkcs1()
with open('private.pem', 'wb') as w_pri:
  w_pri.write(pri)


with open('public.pem', mode='rb') as f, open('private.pem', mode='rb') as f1:
	pub = f.read()
	pri = f1.read()
	pubkey = rsa.PublicKey.load_pkcs1(pub)
	privkey = rsa.PrivateKey.load_pkcs1(pri)
	
message = "123456"
info = rsa.encrypt(message.encode('utf-8'), pubkey)

msg = rsa.decrypt(info, privkey)

print(msg.decode('utf-8'))