from pydantic import BaseModel

class UserLogin(BaseModel):
	login: str
	password: str

class CreateTask(BaseModel):
	task: str
	deadline: str
	complete: bool=False
	token: str
	

class UpdateTask(BaseModel):
	id: int
	task: str
	deadline: str
	complete: bool
	token: str
