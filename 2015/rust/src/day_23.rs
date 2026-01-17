use std::fs;

struct Registers {
    a: u32,
    b: u32,
    pc: usize,
}

fn main() {
    let input: String = fs::read_to_string("inputs/day_23/input.txt").unwrap();
    let lines: Vec<&str> = input.lines().collect();
    let mut reg = Registers::new();
    while reg.pc < lines.len() {
        let instruction_line = &lines[reg.pc];
        execute_instruction(&instruction_line, &mut reg);
    }
    println!("This is day 23 of Advent of Code 2015 in rust!");
    println!("Part 1: b = {}", reg.b);
    reg = Registers::new();
    reg.a = 1;
    while reg.pc < lines.len() {
        let instruction_line = &lines[reg.pc];
        execute_instruction(&instruction_line, &mut reg);
    }
    println!("Part 2: b = {}", reg.b);
}

fn execute_instruction(line: &str, registers: &mut Registers) {
    let instruction = &line[0..3];
    match instruction {
        "hlf" => {
            let register = &line[4..=4];
            match register {
                "a" => registers.a /= 2,
                "b" => registers.b /= 2,
                _ => println!("Non valid register"),
            }
            registers.pc += 1;
        }
        "tpl" => {
            let register = &line[4..=4];
            match register {
                "a" => registers.a *= 3,
                "b" => registers.b *= 3,
                _ => println!("Non valid register"),
            }
            registers.pc += 1;
        }
        "inc" => {
            let register = &line[4..=4];
            match register {
                "a" => registers.a += 1,
                "b" => registers.b += 1,
                _ => println!("Non valid register"),
            }
            registers.pc += 1;
        }
        "jmp" => {
            let sign = &line[4..=4];
            let absolute_offset: usize = line[5..].parse::<usize>().unwrap();
            match sign {
                "+" => registers.pc += absolute_offset,
                "-" => (registers.pc, _) = registers.pc.overflowing_sub(absolute_offset),
                _ => println!("Non valid register"),
            }
        }
        "jie" => {
            let register = &line[4..=4];
            let sign = &line[7..=7];
            let absolute_offset: usize = line[8..].parse::<usize>().unwrap();
            let is_jump;
            match register {
                "a" => is_jump = registers.a % 2 == 0,
                "b" => is_jump = registers.b % 2 == 0,
                _ => {
                    is_jump = false;
                    println!("Non valid register");
                }
            }
            if is_jump {
                match sign {
                    "+" => registers.pc += absolute_offset,
                    "-" => (registers.pc, _) = registers.pc.overflowing_sub(absolute_offset),
                    _ => println!("Non valid register"),
                }
            } else {
                registers.pc += 1;
            }
        }
        "jio" => {
            let register = &line[4..=4];
            let sign = &line[7..=7];
            let absolute_offset: usize = line[8..].parse::<usize>().unwrap();
            let is_jump;
            match register {
                "a" => is_jump = registers.a == 1,
                "b" => is_jump = registers.b == 1,
                _ => {
                    is_jump = false;
                    println!("Non valid register");
                }
            }
            if is_jump {
                match sign {
                    "+" => registers.pc += absolute_offset,
                    "-" => (registers.pc, _) = registers.pc.overflowing_sub(absolute_offset),
                    _ => println!("Non valid register"),
                }
            } else {
                registers.pc += 1;
            }
        }
        _ => println!("Unknonw instruction: {}", line),
    }
}

impl Registers {
    fn new() -> Self {
        Self { a: 0, b: 0, pc: 0 }
    }
}
