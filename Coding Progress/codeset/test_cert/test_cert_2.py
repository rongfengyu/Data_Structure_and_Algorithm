#coding=utf-8

import rsa

from rsa import PublicKey,transform, core

import base64


with open('rsa_public_key.pem', mode='rb') as f, open('rsa_private.pem', mode='rb') as f1:
	pub = f.read()
	pri = f1.read()
	pubkey = rsa.PublicKey.load_pkcs1_openssl_pem(pub)
	privkey = rsa.PrivateKey.load_pkcs1(pri)
	
message = "123456"
info = rsa.encrypt(message.encode('utf-8'), pubkey)

info_baseenc = base64.b64encode(info)

info_basedec = base64.b64decode(info_baseenc)

msg = rsa.decrypt(info_basedec, privkey)

print(msg.decode('utf-8'))
