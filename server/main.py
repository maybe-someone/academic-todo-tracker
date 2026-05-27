from fastapi import FastAPI

app = FastAPI()

@app.get("/")
def read_root():
    return {"message": "Online tracker of tasks is started successfully"}
