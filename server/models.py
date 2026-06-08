from sqlalchemy import Column, Integer, String, Boolean, ForeignKey, DateTime
from sqlalchemy.orm import relationship
from database import Base as db


class User(db):
    __tablename__ = "users"

    id = Column(Integer, primary_key = True)
    username = Column(String, unique = True, index = True, nullable = False)
    hash_password = Column(String, nullable=False)

    tasks = relationship("Task", back_populates = "owner")

class Task(db):
    __tablename__ = "tasks"

    id = Column(Integer, primary_key=True)
    user_id = Column(Integer, ForeignKey("users.id"), nullable=False)

    text = Column(String)
    deadline = Column(DateTime, nullable=True)
    complete = Column(Boolean, default = False)
    favorite = Column(Boolean, default=False)

    owner = relationship("User", back_populates = "tasks")
