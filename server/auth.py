import hashlib
import bcrypt
from database import *

def get_password(password: str):
	return (bcrypt.hashpw(password.encode('utf-8'), bcrypt.gensalt())).decode('utf-8')

def user_reg(login: str, password: str):
	if select_user_db(login):
		return False
	else:
		add_user_db(login, get_password(password))
		return True

def user_auth(login: str, password: str):
	return bcrypt.checkpw(password.encode('utf-8'), select_user_db(login)[-1].encode('utf-8'))
