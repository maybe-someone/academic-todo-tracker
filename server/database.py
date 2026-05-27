import sqlite3 as sq

def init_db():
	#we create a database
	conn = sq.connect('users.db')
	
	#we create a cursor
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


if __name__ == '__main__':
    init_db()
    print("Database is initialized successfully")
