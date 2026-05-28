import bcrypt
from database import *

def get_password(password: str):
	return (bcrypt.hashpw(password.encode('utf-8'), bcrypt.gensalt())).decode('utf-8')

def user_reg(login: str, password: str):
	if select_user_db(login.lower()):
		return False
	else:
		add_user_db(login.lower(), get_password(password))
		return True

def user_auth(login: str, password: str):
	username = select_user_db(login.lower())
	if username:
		return bcrypt.checkpw(password.encode('utf-8'), username.password.encode('utf-8'))
	return False