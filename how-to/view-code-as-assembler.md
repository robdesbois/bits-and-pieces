= Producing assembly =
* Compile with one of the following:
	* `-Wa,-acghl=ofile` to request assembler:
		* -ac:    omit false conditionals
		* -ag:    omit debugging directives
		* -ah:    include high-level source
		* -al:    output assembly listing
		* -as:    include symbol information
		* =ofile: output listing to <ofile> (stdout if omitted)
	* `-S` to produce a `.s` assembly file
* Compile with `-masm=intel` for Intel-syntax assembly
* Compile with `-save-temps` to generate preprocessed listing (`.ii`) and assembly (`.s`)
	* Include `-fverbose-asm` for annotated, more readable assembly (but not for comparing listings)
