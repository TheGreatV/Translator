`{
This sample show scope hierarchy inside schema algorithms
}`

.schema {
	.algorithm .none .body; `/ keyword "algorithm" generate new unnamed scope, used for containing algorithm parameters
}

.schema {
	.make a .of A;
	.algorithm .none(.make a .of A) .body; `/ instance "a" declared in the scope of algorithm (no collision)
}









