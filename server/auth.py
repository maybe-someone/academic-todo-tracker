import bcrypt
from database import *
import jwt
import datetime

SECRET_KEY = "super-safe-secret-key-change-me-also-i-love-pizza-and-cats"
ALGORITHM = "HS256"

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
		db_password_hash = username[1].encode('utf-8')
		user_password_bytes = password.encode('utf-8')
		return bcrypt.checkpw(user_password_bytes, db_password_hash)
	return False

def user_token_create(login: str):
	pi = {"user": login, "exp": datetime.datetime.now(datetime.timezone.utc) + datetime.timedelta(days=1)}
	token = jwt.encode(pi, SECRET_KEY, algorithm = ALGORITHM)
	return token

def access(token: str):
	try:
		login = jwt.decode(token, SECRET_KEY, algorithms=[ALGORITHM])
		return login.get("user")
	except (jwt.ExpiredSignatureError, jwt.InvalidTokenError):
		return None