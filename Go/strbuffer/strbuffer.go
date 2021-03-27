package strbuffer

// ChosenString returns the selected string
func ChosenString(check bool) string {
	if check {
		return "Amo"
	}
	return "Jay"
}

// Calculate returns the result
func Calculate(number int) int {

	var result int
	if number%2 == 0 {
		number /= 2
	} else {
		number *= 2
	}
	result = int(number)
	return result
}
