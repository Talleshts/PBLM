import random


class LineBalancing:
    def __init__(self, instance_file):
        self.num_tasks = 0
        self.task_times = []
        self.precedences = []
        self.read_instance(instance_file)
        self.best_solution = None

    def read_instance(self, instance_file):
        with open(instance_file, 'r') as file:
            lines = file.readlines()
            self.num_tasks = int(lines[0].strip())
            self.task_times = [int(line.strip()) for line in lines[1:self.num_tasks+1]]
            self.precedences = [(int(pair.split(',')[0]), int(pair.split(',')[1])) for pair in lines[self.num_tasks+2:-1]]

    def create_initial_solution(self):
        sorted_tasks = sorted(range(self.num_tasks), key=lambda x: self.task_times[x])
        line = [[] for _ in range(3, 12)]  # Number of machines ranging from 3 to 11
        for task in sorted_tasks:
            machine = min(range(len(line)), key=lambda x: sum(self.task_times[t] for t in line[x]))
            line[machine].append(task)
        return line

    def calculate_makespan(self, line):
        return max(sum(self.task_times[task] for task in machine) for machine in line)

    def write_solution(self, line):
        for i, machine in enumerate(line, start=1):
            tasks_str = ', '.join(str(task + 1) for task in machine)
            print(f"Máquina {i}: {tasks_str} (tarefas atendidas)")
        makespan = self.calculate_makespan(line)
        print(f"FO: {makespan} (soma dos custos das tarefas da máquina que possui o maior custo)")

    def apply_local_search(self, line):
        best_line = line
        best_makespan = self.calculate_makespan(line)
        num_machines = len(line)

        for _ in range(100):
            # Select a random task
            task = random.randint(0, self.num_tasks - 1)

            # Find the machine that currently contains the task
            machine_index = next((i for i, machine in enumerate(line) if task in machine), None)
            if machine_index is None:
                continue

            # Remove the task from the machine
            line[machine_index].remove(task)

            # Calculate the makespan for each possible machine
            makespans = []
            for m in range(num_machines):
                line[m].append(task)
                makespans.append(self.calculate_makespan(line))
                line[m].remove(task)

            # Find the machine with the minimum makespan
            min_makespan = min(makespans)
            best_machines = [m for m, makespan in enumerate(makespans) if makespan == min_makespan]

            # Randomly select one of the best machines
            new_machine = random.choice(best_machines)
            line[new_machine].append(task)

            # Update the best solution if necessary
            new_makespan = self.calculate_makespan(line)
            if new_makespan < best_makespan:
                best_line = [list(machine) for machine in line]
                best_makespan = new_makespan

        return best_line

    def run(self):
        for num_machines in range(3, 12):
            line = self.create_initial_solution()
            best_makespan = self.calculate_makespan(line)
            for _ in range(1000):
                new_line = self.apply_local_search(line)
                new_makespan = self.calculate_makespan(new_line)
                if new_makespan < best_makespan:
                    line = new_line
                    best_makespan = new_makespan
            if self.best_solution is None or best_makespan < self.calculate_makespan(self.best_solution):
                self.best_solution = line
        self.write_solution(self.best_solution)


# Usage example
lb = LineBalancing("instancia.txt")
lb.run()
