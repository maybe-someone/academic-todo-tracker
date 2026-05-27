import sqlite3 as sq

#func for initialize a database
def init_db():
	conn = sq.connect('usersAndTasks.db')	
	cursor = conn.cursor()
	cursor.execute("PRAGMA foreign_keys = ON")
	
	cursor.execute("""CREATE TABLE IF NOT EXISTS users (
		login TEXT PRIMARY KEY,
		password_hash TEXT
	)""")
	
	cursor.execute("""CREATE TABLE IF NOT EXISTS tasks (
		id INTEGER PRIMARY KEY AUTOINCREMENT,
		login TEXT,
		task TEXT,
		deadline TEXT,
		complete INTEGER,
		FOREIGN KEY (login) REFERENCES users (login)
	)""")
	
	conn.commit()
	conn.close()

#func for select in task's database
def select_task_db(login):
	conn = sq.connect('usersAndTasks.db')
	cursor = conn.cursor()
	cursor.execute("PRAGMA foreign_keys = ON")
	
	cursor.execute("SELECT * FROM tasks WHERE login = ?", (login,))
	
	all_tasks = cursor.fetchall()
	conn.close()
	
	return all_tasks


#remove complete a task in databse
def remover_task_db(id):
	conn = sq.connect('usersAndTasks.db')
	cursor = conn.cursor()
	cursor.execute("PRAGMA foreign_keys = ON")
	
	cursor.execute("DELETE FROM tasks WHERE id = ?", (id,))
	
	conn.commit()
	conn.close()

#updater of tasks
def upd_task_db(id, task, deadline, complete):
	conn = sq.connect('usersAndTasks.db')
	cursor = conn.cursor()
	cursor.execute("PRAGMA foreign_keys = ON")
	
	cursor.execute("""UPDATE tasks SET
		task = ?,
		deadline = ?,
		complete = ?
		WHERE id = ?
	""", (task, deadline, complete, id))
	
	conn.commit()
	conn.close()

#adder of tasks
def add_task_db(login, task, deadline, complete=0):
	conn = sq.connect('usersAndTasks.db')
	cursor = conn.cursor()
	cursor.execute("PRAGMA foreign_keys = ON")
	
	cursor.execute("""INSERT INTO tasks(
		login,
		task,
		deadline,
		complete
		) VALUES (?, ?, ?, ?)""",
	(login, task, deadline, complete))
	
	conn.commit()
	conn.close()

#adder of users
def add_user_db(login, password_hash):
        conn = sq.connect('usersAndTasks.db')
        cursor = conn.cursor()
	cursor.execute("PRAGMA foreign_keys = ON")
	
        cursor.execute("""INSERT INTO users(
                login,
                password_hash
                ) VALUES (?, ?)""",
        (login, password_hash))
	
        conn.commit()
        conn.close()


if __name__ == '__main__':
	init_db()
	print("Database is initialized successfully")
	
	all_task = select_task_db()
	print("Select task is worked seccessfully. Task:", all_task)
	
	remover_task_db()
	print("Remover is OK")
	
	upd_task_db()
	print("Updater is OK")
