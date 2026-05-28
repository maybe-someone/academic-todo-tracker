from fastapi import FastAPI, HTTPException, status
from database import *
from pydantic import BaseModel
from contextlib import asynccontextmanager
from schemas import UserLogin, CreateTask
from auth import user_reg, user_auth



@asynccontextmanager
async def lifespan(app: FastAPI):
	init_db()
	yield
	pass

app = FastAPI()


@app.post("/register", status_code=status.HTTP_201_CREATED)
def reg_user(user: UserLogin):
	if user_reg(user.login, user.password):
		return {"status": "Registration completed!"}
	else:
		raise HTTPException(
            status_code=status.HTTP_409_CONFLICT,
            detail="This login is already taken."
        )

@app.post("/login")
def login_user(user: UserLogin):
	if user_auth(user.login, user.password):
		return {"status": "Successful login!"}
	else:
		raise HTTPException(
            status_code=status.HTTP_401_UNAUTHORIZED,
            detail="Incorrect login or password."
        )



@app.get("/")
def read_root():
    return {"status": "Server is running"}

