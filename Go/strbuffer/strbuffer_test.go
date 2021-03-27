package strbuffer_test

import (
	"testing"

	"example.com/user/hello/strbuffer"
)

func TestString(t *testing.T) {
	if strbuffer.ChosenString(false) != "Jay" {
		t.Fatal("Wrong name!")
	}
}
