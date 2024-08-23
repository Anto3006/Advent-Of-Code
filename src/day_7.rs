fn main() {
    println!("This is day 7 of advent of code 2015 in Rust");
}

struct Wire {
    value: u32,
}

enum Gate {
    Not(Wire),
    And(Wire, Wire),
    Or(Wire, Wire),
    LShift(Wire, Wire),
    RShift(Wire, Wire),
}
