from fastapi import FastAPI
from database import *
from pydantic import BaseModel
from contextlib import asynccontextmanager

@asynccontextmanager
asenc def lifespan(app: FastAPI):
	init.db()
	yield
	pass





app = FastAPI()

@app.get("/")
def read_root():
    return {"status": "Server is running"}

