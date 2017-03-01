.make + .of .schema {
	.algorithm Type .left .make x .of Type .right .make y .of Type .body;
}
.make * .of .schema {
	.algorithm Type .left .make x .of Type .right .make y .of Type .body;
}

a + c * d; `/ order is undefined

.priority *; `/ instance * obtain arguments first
.priority .left .right *; `/ algorith with left and right arguments, of instance *, obtain arguments first

a + c * d; `/ c*d goes first: a + (c * d)


















