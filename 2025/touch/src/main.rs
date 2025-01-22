use std::fs::{File, OpenOptions};
use std::io::{self, BufRead, Write};
use std::path::Path;

#[derive(Debug)]
struct Task {
    description: String,
    completed: bool,
}

impl Task {
    fn new(description: String) -> Task {
        Task {
            description,
            completed: false,
        }
    }
}

fn load_tasks(file_path: &str) -> Vec<Task> {
    let mut tasks = Vec::new();

    if let Ok(lines) = read_lines(file_path) {
        for line in lines {
            if let Ok(task_str) = line {
                let parts: Vec<&str> = task_str.split("|").collect();
                if parts.len() == 2 {
                    let description = parts[0].to_string();
                    let completed = parts[1] == "true";
                    tasks.push(Task { description, completed });
                }
            }
        }
    }

    tasks
}

fn save_tasks(file_path: &str, tasks: &Vec<Task>) -> io::Result<()> {
    let mut file = OpenOptions::new()
        .write(true)
        .truncate(true)
        .create(true)
        .open(file_path)?;

    for task in tasks {
        writeln!(file, "{}|{}", task.description, task.completed)?;
    }

    Ok(())
}

fn read_lines<P>(filename: P) -> io::Result<io::Lines<io::BufReader<File>>>
where
    P: AsRef<Path>,
{
    let file = File::open(filename)?;
    Ok(io::BufReader::new(file).lines())
}

fn print_tasks(tasks: &Vec<Task>) {
    println!("Tasks:");
    for (index, task) in tasks.iter().enumerate() {
        let status = if task.completed { "[X]" } else { "[ ]" };
        println!("{} {}: {}", index + 1, status, task.description);
    }
}

fn main() {
    let file_path = "tasks.txt";
    let mut tasks = load_tasks(file_path);

    loop {
        println!("\nWhat would you like to do?");
        println!("1. Add a task");
        println!("2. View tasks");
        println!("3. Mark a task as completed");
        println!("4. Delete a task");
        println!("5. Exit");

        let mut choice = String::new();
        io::stdin()
            .read_line(&mut choice)
            .expect("Failed to read input");

        match choice.trim() {
            "1" => {
                println!("Enter the task description:");
                let mut description = String::new();
                io::stdin()
                    .read_line(&mut description)
                    .expect("Failed to read input");
                let task = Task::new(description.trim().to_string());
                tasks.push(task);
                save_tasks(file_path, &tasks).expect("Failed to save tasks");
                println!("Task added!");
            }
            "2" => {
                print_tasks(&tasks);
            }
            "3" => {
                print_tasks(&tasks);
                println!("Enter the task number to mark as completed:");
                let mut task_num = String::new();
                io::stdin()
                    .read_line(&mut task_num)
                    .expect("Failed to read input");
                if let Ok(num) = task_num.trim().parse::<usize>() {
                    if num > 0 && num <= tasks.len() {
                        tasks[num - 1].completed = true;
                        save_tasks(file_path, &tasks).expect("Failed to save tasks");
                        println!("Task marked as completed!");
                    } else {
                        println!("Invalid task number!");
                    }
                } else {
                    println!("Invalid input!");
                }
            }
            "4" => {
                print_tasks(&tasks);
                println!("Enter the task number to delete:");
                let mut task_num = String::new();
                io::stdin()
                    .read_line(&mut task_num)
                    .expect("Failed to read input");
                if let Ok(num) = task_num.trim().parse::<usize>() {
                    if num > 0 && num <= tasks.len() {
                        tasks.remove(num - 1);
                        save_tasks(file_path, &tasks).expect("Failed to save tasks");
                        println!("Task deleted!");
                    } else {
                        println!("Invalid task number!");
                    }
                } else {
                    println!("Invalid input!");
                }
            }
            "5" => {
                println!("Goodbye!");
                break;
            }
            _ => {
                println!("Invalid choice!");
            }
        }
    }
}