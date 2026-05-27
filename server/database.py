import sqlite3 as sq

#func for initialize a database
def init_db():
	conn = sq.connect('usersAndTasks.db')	
	cursor = conn.cursor()
	
	cursor.execute("""CREATE TABLE IF NOT EXISTS users (
		login TEXT PRIMARY KEY,
		password_hash TEXT
	)""")
	
	cursor.execute("""CREATE TABLE IF NOT EXISTS tasks (
		id INTEGER PRIMARY KEY AUTOINCREMENT,
		login TEXT,
		task TEXT,
		deadline TEXT,
		FOREIGN KEY (login) REFERENCES users (login)
	)""")

	conn.commit()
	conn.close()

#func for select in task's database
def select_task_db(login):
	conn = sq.connect('usersAndTasks.db')
	cursor = conn.cursor()
	
	cursor.execute("SELECT * FROM tasks WHERE login = ?", (login,))
	
	all_tasks = cursor.fetchall()
	conn.close()
	
	return all_tasks


#remove complete a task in databse
def remover_task_db(id):
	conn = sq.connect('usersAndTasks.db')
	cursor = conn.cursor()
	
	cursor.execute("DELETE FROM tasks WHERE id = ?", (id,))
	
	conn.commit()
	conn.close()

if __name__ == '__main__':
	init_db()
	print("Database is initialized successfully")
	
	all_task = select_task_db()
	print("Select task is worked seccessfully. Task:", all_task)
