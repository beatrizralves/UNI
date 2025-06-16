# Task Management System

This is a console-based system written in C that allows users to manage **tasks** and **collaborators**. It supports creating, editing, and tracking tasks of various types, while also handling collaborator records including contact information, birthdate, and more. Data persistence is ensured through text and binary files.

## Features

### ✅ Task Management

* Create tasks with categories: *School*, *Professional*, *Personal*, *Family*, *Leisure*
* Edit and delete existing tasks
* Mark tasks as completed
* List tasks:

  * All
  * By type
  * Completed tasks
  * Tasks for today
  * Tasks after a specific date
  * Tasks in a date interval
* Dynamically add new task types

### 👤 Collaborator Management

* Add new collaborators with:

  * ID (auto-incremented)
  * Name, phone number, birthdate, email, and address
* List all collaborators
* Search by ID
* Edit collaborator data
* Remove collaborators from the system
* Link a collaborator to a task

### 💾 File Storage

* Tasks are stored in a binary file `tarefa.bin`
* Collaborators are stored in a text file `Colaboradores.txt`
* Data is loaded and saved automatically

## Structure

* `main.c`: Main application logic with task and collaborator menu
* `funcoes.h`, `funcoes.c`: Functions for managing tasks and collaborators
* `struct.h`: Structure definitions for `TAREFA` and `colab`
