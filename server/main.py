from fastapi import FastAPI
from database import *
from pydantic import BaseModel
from contextlib import asynccontextmanager
from schemas import




@asynccontextmanager
asenc def lifespan(app: FastAPI):
	init.db()
	yield
	pass


@app.post("/register")
def user_reg(user: UserLogin):
	name = user.login
	password = user.password
	add_user_db(name, password)

app = FastAPI()

@app.get("/")
def read_root():
    return {"status": "Server is running"}

