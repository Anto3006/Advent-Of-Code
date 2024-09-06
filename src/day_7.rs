use aoc;
use std::collections::HashMap;
use std::fs;

fn main() {
    println!("This is day 7 of advent of code 2015 in Rust");
    let input = fs::read_to_string("inputs/day_7/part_2.txt").unwrap();
    let mut c = Circuit::default();
    c.read_circuit(input);
    for wire in c.wires.iter() {
        println!("{:?}", wire);
    }
    let mut n = 0;
    while !c.activate_gates() {
        println!("Iter: {n}");
        n += 1;
    }
    println!("{:?}", c.wires.get("a").unwrap())
}

#[derive(Debug)]
enum GateType {
    Not,
    And,
    Or,
    LShift,
    RShift,
    None,
}

#[derive(Debug)]
enum Signal {
    Wire(String),
    Constant(u16),
}

impl Signal {
    fn get_value(&self, wires: &HashMap<String, u16>) -> u16 {
        match self {
            Self::Constant(value) => *value,
            Self::Wire(wire) => *wires.get(wire).unwrap_or(&0),
        }
    }

    fn from_value(value: &String) -> Self {
        if aoc::is_numeric(value) {
            Self::Constant(value.parse().unwrap())
        } else {
            Self::Wire(value.to_string())
        }
    }
}

#[derive(Debug)]
struct Gate {
    gate_type: GateType,
    inputs: Vec<Signal>,
    output: String,
}

impl Gate {
    fn get_output(&self, wires: &HashMap<String, u16>) -> u16 {
        match self.gate_type {
            GateType::Not => {
                let input_value = self.inputs[0].get_value(wires);
                !input_value
            }
            GateType::And => {
                let input_1 = self.inputs[0].get_value(wires);
                let input_2 = self.inputs[1].get_value(wires);
                input_1 & input_2
            }
            GateType::Or => {
                let input_1 = self.inputs[0].get_value(wires);
                let input_2 = self.inputs[1].get_value(wires);
                input_1 | input_2
            }
            GateType::LShift => {
                let input_1 = self.inputs[0].get_value(wires);
                let input_2 = self.inputs[1].get_value(wires);
                input_1 << input_2
            }
            GateType::RShift => {
                let input_1 = self.inputs[0].get_value(wires);
                let input_2 = self.inputs[1].get_value(wires);
                input_1 >> input_2
            }
            GateType::None => {
                let input_value = self.inputs[0].get_value(wires);
                input_value
            }
        }
    }

    fn update_ouput(&self, wires: &mut HashMap<String, u16>) {
        let output = self.get_output(wires);
        *wires.entry(self.output.clone()).or_insert(0) = output;
    }
}

#[derive(Default)]
struct Circuit {
    wires: HashMap<String, u16>,
    is_wire_activated: HashMap<String, bool>,
    gates: Vec<Gate>,
}

impl Circuit {
    fn activate_gates(&mut self) -> bool {
        let mut all_gates_activated = true;
        for gate in self.gates.iter() {
            let mut can_gate_activate = true;
            for wire in gate.inputs.iter() {
                if let Signal::Wire(wire) = wire {
                    if !*self.is_wire_activated.get(wire).unwrap_or(&false) {
                        can_gate_activate = false;
                    }
                }
            }
            if can_gate_activate {
                gate.update_ouput(&mut self.wires);
                *self
                    .is_wire_activated
                    .entry(gate.output.to_string())
                    .or_insert(false) = true;
            } else {
                all_gates_activated = false;
            }
        }
        all_gates_activated
    }

    fn read_circuit(&mut self, text: String) {
        for line in text.lines() {
            let data = aoc::split_string(line, " -> ");
            let output = data.get(1);
            if let Some(output) = output {
                self.wires.entry(output.to_string()).or_insert(0);
                self.is_wire_activated
                    .entry(output.to_string())
                    .or_insert(false);
                let input_data = aoc::split_string(&data[0], " ");
                if input_data.len() == 1 {
                    if aoc::is_numeric(&input_data[0]) {
                        *self.wires.entry(output.to_string()).or_insert(0) =
                            input_data[0].parse().unwrap();
                        *self
                            .is_wire_activated
                            .entry(output.to_string())
                            .or_insert(false) = true;
                    } else {
                        let value = Signal::from_value(&input_data[0]);
                        if let Signal::Wire(wire) = &value {
                            self.wires.entry(wire.to_string()).or_insert(0);
                            self.is_wire_activated
                                .entry(wire.to_string())
                                .or_insert(false);
                        }
                        let gate = Gate {
                            gate_type: GateType::None,
                            inputs: vec![value],
                            output: output.to_string(),
                        };
                        self.gates.push(gate);
                    }
                } else if input_data[0] == "NOT" {
                    let value = Signal::from_value(&input_data[1]);
                    if let Signal::Wire(wire) = &value {
                        self.wires.entry(wire.to_string()).or_insert(0);
                        self.is_wire_activated
                            .entry(wire.to_string())
                            .or_insert(false);
                    }
                    let gate = Gate {
                        gate_type: GateType::Not,
                        inputs: vec![value],
                        output: output.to_string(),
                    };
                    self.gates.push(gate);
                } else if input_data[1] == "AND" {
                    let value_1 = Signal::from_value(&input_data[0]);
                    let value_2 = Signal::from_value(&input_data[2]);
                    if let Signal::Wire(wire) = &value_1 {
                        self.wires.entry(wire.to_string()).or_insert(0);
                        self.is_wire_activated
                            .entry(wire.to_string())
                            .or_insert(false);
                    }
                    if let Signal::Wire(wire) = &value_2 {
                        self.wires.entry(wire.to_string()).or_insert(0);
                        self.is_wire_activated
                            .entry(wire.to_string())
                            .or_insert(false);
                    }
                    let gate = Gate {
                        gate_type: GateType::And,
                        inputs: vec![value_1, value_2],
                        output: output.to_string(),
                    };
                    self.gates.push(gate);
                } else if input_data[1] == "OR" {
                    let value_1 = Signal::from_value(&input_data[0]);
                    let value_2 = Signal::from_value(&input_data[2]);
                    if let Signal::Wire(wire) = &value_1 {
                        self.wires.entry(wire.to_string()).or_insert(0);
                        self.is_wire_activated
                            .entry(wire.to_string())
                            .or_insert(false);
                    }
                    if let Signal::Wire(wire) = &value_2 {
                        self.wires.entry(wire.to_string()).or_insert(0);
                        self.is_wire_activated
                            .entry(wire.to_string())
                            .or_insert(false);
                    }
                    let gate = Gate {
                        gate_type: GateType::Or,
                        inputs: vec![value_1, value_2],
                        output: output.to_string(),
                    };
                    self.gates.push(gate);
                } else if input_data[1] == "LSHIFT" {
                    let value_1 = Signal::from_value(&input_data[0]);
                    let value_2 = Signal::from_value(&input_data[2]);
                    if let Signal::Wire(wire) = &value_1 {
                        self.wires.entry(wire.to_string()).or_insert(0);
                        self.is_wire_activated
                            .entry(wire.to_string())
                            .or_insert(false);
                    }
                    if let Signal::Wire(wire) = &value_2 {
                        self.wires.entry(wire.to_string()).or_insert(0);
                        self.is_wire_activated
                            .entry(wire.to_string())
                            .or_insert(false);
                    }
                    let gate = Gate {
                        gate_type: GateType::LShift,
                        inputs: vec![value_1, value_2],
                        output: output.to_string(),
                    };
                    self.gates.push(gate);
                } else if input_data[1] == "RSHIFT" {
                    let value_1 = Signal::from_value(&input_data[0]);
                    let value_2 = Signal::from_value(&input_data[2]);
                    if let Signal::Wire(wire) = &value_1 {
                        self.wires.entry(wire.to_string()).or_insert(0);
                        self.is_wire_activated
                            .entry(wire.to_string())
                            .or_insert(false);
                    }
                    if let Signal::Wire(wire) = &value_2 {
                        self.wires.entry(wire.to_string()).or_insert(0);
                        self.is_wire_activated
                            .entry(wire.to_string())
                            .or_insert(false);
                    }
                    let gate = Gate {
                        gate_type: GateType::RShift,
                        inputs: vec![value_1, value_2],
                        output: output.to_string(),
                    };
                    self.gates.push(gate);
                }
            }
        }
    }
}
