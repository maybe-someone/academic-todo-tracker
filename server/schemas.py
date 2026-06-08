from pydantic import BaseModel, ConfigDict
from datetime import datetime
from typing import Optional

class AuthUser(BaseModel): #get
    username: str
    password: str

class UserAuthResponse(BaseModel): #send
    user_id: int
    model_config = ConfigDict(from_attributes=True)

class TaskShow(BaseModel): #send
    id: int 
    text: str
    deadline: Optional[datetime] = None
    complete: bool
    favorite: bool
    model_config = ConfigDict(from_attributes=True)

class CreateTask(BaseModel): #get
    text: str
    deadline: Optional[datetime] = None
    complete: bool = False
    favorite: bool = False

# class DeleteTask(BaseModel): #get
#     id: int
#     favorite: bool = False

class UpgradeTask(BaseModel): #get
    id: int
    text: Optional[str] = None
    deadline: Optional[datetime] = None
    complete: Optional[bool] = None
    favorite: Optional[bool] = None