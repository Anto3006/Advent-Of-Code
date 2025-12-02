fn main() {
    println!("This is day 20 of Advent of Code 2015 in rust");
    let target: u32 = 33100000;
    let mut house_p1 = 1;
    while sum_of_divisors(house_p1, None) * 10 < target {
        house_p1 += 1;
    }
    println!("Part 2:{house_p1}");
    let mut house_p2 = 1;
    while sum_of_divisors(house_p2, Some(50)) * 11 < target {
        house_p2 += 1;
    }
    println!("Part 2:{house_p2}");
}

fn sum_of_divisors(n: u32, max_repeat: Option<u32>) -> u32 {
    let mut sum = 0;
    let mut divisor = 1;
    while divisor * divisor <= n {
        if n % divisor == 0 {
            if let Some(max_repeat) = max_repeat {
                if n <= max_repeat * divisor {
                    sum += divisor;
                }
                if n * divisor <= 50 * n {
                    sum += n / divisor;
                }
            } else {
                sum += divisor + n / divisor;
            }
        }
        divisor += 1;
    }
    sum
}
