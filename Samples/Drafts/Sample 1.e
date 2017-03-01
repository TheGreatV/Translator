`/ comment line
`{ comment block }`

`/ blocks
`{ block can contain any kind of instructions }`
{} `/ unnamed block
.block {} `/ unnamed block
.block @ {} `/ named block

`/ aliasing
`{ aliasing bind identifier to named object }`
.alias a .of b; `/ 'a' is a NEW identifier for previously defined 'b'

`/ space
`{ space can contain only declarations }`
.space {} `/ unnamed space
.space # {} `/ named space

`/ schema
.schema {} `/ unnamed schema
.schema S {} `/ named schema

`/ instance
.make S; `/ unnamed instance of schema 'S'
.make s .of S; `/ named instance 's' of schema 'S'

`/ pointers
.make .location S;
.make p .of .location S;

`/ dereferencing
.value p;

`/ calls
.make Property .of .schema .algorithm .none .body {} `/ executes when instance is called by name
Property;

.make Function .of .schema .algorithm .none () .body {} `/ executes when instance is called with braces
Function();

.make Function .of .schema .algorithm R (.make A) .body { `{ TBD }` } `/ variant with result and arguments
Function(a);

.make Prefix .of .schema .algorithm .none .right .make A .body {} `/ when right operand is type-equal
Prefix .make A;

.make Postfix .of .schema .algorithm .none .left .make A .body {} `/ when left operand is type-equal
.make A Postfix;

.make Interfix .of .schema .algorithm .none .left .make A .right .make B .body {} `/ when left operand is type-equal
.make A Interfix .make B;




