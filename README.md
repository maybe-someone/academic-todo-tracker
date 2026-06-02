# academic-todo-tracker

This repository contains a full-stack Task Manager application comprised of a Python-based FastAPI backend service and a C++ command-line interface (CLI) client application.

---

## Repository Structure

* `main.py`: The entry point for the FastAPI web server, defining application routes and middleware.
* `database.py`: Handles SQLite database initialization, sessions, and raw SQL queries for users and tasks.
* `auth.py`: Implements user authentication logic, including password hashing with `bcrypt` and JWT token handling.
* `schemas.py`: Defines Pydantic models for request validation and serialization.
* `Dockerfile`: Provides containerization instructions for the Python backend application.
* `requirements.txt`: Specifies the Python dependencies necessary to run the web server.
* `main.cpp`: The entry point for the C++ CLI client interface.

---

## Server Architecture (Python / FastAPI)

The backend is built using FastAPI and stores data persistently inside an SQLite database (`usersAndTasks.db`). Authentication is managed via JSON Web Tokens (JWT) using the OAuth2 password bearer schema.

### API Endpoints

#### Authentication

* `POST /register`: Registers a new user. Expects a JSON body containing `login` and `password`. Returns an initial access token upon completion.
* `POST /login`: Authenticates an existing user. Expects form-encoded data (`username` and `password`). Returns a bearer token.

#### Tasks

* `GET /tasks`: Retrieves all tasks belonging to the authenticated user.
* `POST /tasks/add`: Creates a new task. Requires a authenticated bearer token.
* `PUT /tasks/update`: Updates an existing task by its unique identifier. Requires token ownership validation.
* `DELETE /tasks/remove`: Deletes a task by identifier. Requires token ownership validation.

#### Utility

* `GET /`: Health check endpoint indicating the server status.

---

## Client Architecture (C++)

The client is a console-based application interacting with the FastAPI server using an HTTP client library (`httplib`).

### Execution Flow

1. **Server Connection**: Establishes a connection to the specified server URL (passed via command-line arguments or defaulting to `http://localhost:8000`).
2. **Authentication Loop**: Requires the user to log in or register before accessing core functionalities. Successful operations populate the request headers with the authorization token.
3. **Main Menu**: Provides an interactive loop to view, add, modify, or delete tasks assigned to the authenticated account.

---

## Deployment and Setup

### Running the Server locally

1. Create a `.env` file in the root directory and define the required secret key variable:
```env
SECRET_KEY=your_secure_random_secret_string

```


2. Install the necessary Python packages:
```bash
pip install -r requirements.txt

```


3. Launch the server application via Uvicorn:
```bash
uvicorn main:app --host 0.0.0.0 --port 8000

```



### Running the Server via Docker

1. Ensure a `.env` file containing `SECRET_KEY` is present in the working directory.
2. Build the Docker container image:
```bash
docker build -t task-manager-backend .

```


3. Instantiate and run the container:
```bash
docker run -p 8000:8000 --env-file .env task-manager-backend

```



### Building and Running the C++ Client

1. Ensure that the external dependency `httplib.h` and the local headers `Task_funcs.h` and `Base_modules.h` are accessible within your compiler include path.
2. Compile the executable using a C++11 compliant compiler or higher:
```bash
g++ -std=c++11 main.cpp -o task_client

```


3. Execute the client binary, optionally passing the destination server address:
```bash
./task_client http://localhost:8000

```
