from fastapi import FastAPI, HTTPException, status, Depends
from database import *
from pydantic import BaseModel
from contextlib import asynccontextmanager
from schemas import UserLogin, CreateTask, UpdateTask
from auth import user_reg, user_auth, access, user_token_create
from fastapi.security import OAuth2PasswordBearer, OAuth2PasswordRequestForm


@asynccontextmanager
async def lifespan(app: FastAPI):
	init_db()
	yield
	pass

app = FastAPI(lifespan = lifespan)

oauth2_scheme = OAuth2PasswordBearer(tokenUrl="login")

@app.post("/register", status_code=status.HTTP_201_CREATED)
def reg_user(username: UserLogin):
	if user_reg(username.login, username.password):
		return {"status": "Registration completed!", "token": user_token_create(username.login)}
	else:
		raise HTTPException(
            status_code=status.HTTP_409_CONFLICT,
            detail="This login is already taken."
        )

@app.post("/login")
def login_user(user: OAuth2PasswordRequestForm = Depends()):
	if user_auth(user.username, user.password):
		return {"status": "Successful login!", "access_token": user_token_create(user.username), "token_type": "bearer"}
	else:
		raise HTTPException(
            status_code=status.HTTP_401_UNAUTHORIZED,
            detail="Incorrect login or password."
        )

@app.post("/tasks/add", status_code=status.HTTP_201_CREATED)
def task_add(user: CreateTask, token: str = Depends(oauth2_scheme)):
	token_owner = access(token)
	if not token_owner:
		raise HTTPException(
            status_code=status.HTTP_401_UNAUTHORIZED,
            detail="Token was not accepted."
        )
	if select_user_db(token_owner):
		add_task_db(
			token_owner,
			user.task,
			user.deadline,
			user.complete
		)
		return {"task": "Task created successfully!"}
	else:
		raise HTTPException(
            status_code=status.HTTP_404_NOT_FOUND,
            detail=f"User with login '{token_owner}' does not exist. Register first."
        )

@app.delete("/tasks/remove")
def remove_task(id: int, token: str = Depends(oauth2_scheme)):
	token_owner = access(token)
	if not token_owner:
		raise HTTPException(
            status_code=status.HTTP_401_UNAUTHORIZED,
            detail="Token was not accepted."
        )
	
	task = select_task_db(id)

	if not task:
		raise HTTPException(
            status_code=status.HTTP_404_NOT_FOUND,
            detail=f"Task with id '{id}' does not exist."
        )

	if token_owner != task[1]:
		raise HTTPException(
            status_code=status.HTTP_403_FORBIDDEN,
            detail="You do not have permission to delete this task."
        )

	remover_task_db(id)
	return {"task": "Task removed successfully!"}



@app.put("/tasks/update")
def update_task(task: UpdateTask, token: str = Depends(oauth2_scheme)):
	token_owner = access(token)
	if not token_owner:
		raise HTTPException(
            status_code=status.HTTP_401_UNAUTHORIZED,
            detail="Token was not accepted."
        )

	tasks = select_task_db(task.id)

	if not tasks:
		raise HTTPException(
            status_code=status.HTTP_404_NOT_FOUND,
            detail=f"Task with id '{task.id}' does not exist."
        )

	if token_owner != tasks[1]:
		raise HTTPException(
            status_code=status.HTTP_403_FORBIDDEN,
            detail="You do not have permission to update this task."
        )

	upd_task_db(task.id, task.task, task.deadline, task.complete)
	return {"task": "Task update successfully!"}

@app.get("/tasks")
def get_tasks(token: str = Depends(oauth2_scheme)):
	token_owner = access(token)
	if not token_owner:
		raise HTTPException(
            status_code=status.HTTP_401_UNAUTHORIZED,
            detail="Token was not accepted."
        )
	
	raw_tasks = select_tasks_db(token_owner)

	tasks = []
	for row in raw_tasks:
		tasks.append({
			"id": row[0],
			"login": row[1],
			"task": row[2],
			"deadline": row[3],
			"complete": bool(row[4]),
		})


	return {"tasks": tasks}


@app.get("/")
def read_root():
    return {"status": "Server is running"}

