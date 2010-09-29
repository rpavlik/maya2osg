//Maya ASCII 2010 scene
//Name: test_particles.ma
//Last modified: Thu, Sep 30, 2010 12:25:38 AM
//Codeset: 1252
requires maya "2010";
currentUnit -l centimeter -a degree -t film;
fileInfo "application" "maya";
fileInfo "product" "Maya Unlimited 2010";
fileInfo "version" "2010";
fileInfo "cutIdentifier" "200907280007-756013";
fileInfo "osv" "Microsoft Windows Vista  (Build 7600)\n";
createNode transform -s -n "persp";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 28.000000000000007 21 28 ;
	setAttr ".r" -type "double3" -27.938352729602379 44.999999999999964 0 ;
	setAttr ".rp" -type "double3" -1.7763568394002505e-015 -8.8817841970012523e-016 
		0 ;
	setAttr ".rpt" -type "double3" -2.0254117314783439e-015 1.8443966591649209e-015 
		2.6735708427391984e-015 ;
createNode camera -s -n "perspShape" -p "persp";
	setAttr -k off ".v" no;
	setAttr ".fl" 34.999999999999986;
	setAttr ".coi" 44.82186966202994;
	setAttr ".imn" -type "string" "persp";
	setAttr ".den" -type "string" "persp_depth";
	setAttr ".man" -type "string" "persp_mask";
	setAttr ".hc" -type "string" "viewSet -p %camera";
createNode transform -s -n "top";
	setAttr ".v" no;
	setAttr ".t" -type "double3" -1.8764296830359193 100.45003678382518 -5.9604623098466309e-008 ;
	setAttr ".r" -type "double3" -89.999999999999986 0 0 ;
createNode camera -s -n "topShape" -p "top";
	setAttr -k off ".v" no;
	setAttr ".rnd" no;
	setAttr ".coi" 100.1;
	setAttr ".ow" 20.264177914453516;
	setAttr ".imn" -type "string" "top";
	setAttr ".den" -type "string" "top_depth";
	setAttr ".man" -type "string" "top_mask";
	setAttr ".hc" -type "string" "viewSet -t %camera";
	setAttr ".o" yes;
createNode transform -s -n "front";
	setAttr ".v" no;
	setAttr ".t" -type "double3" -0.1294762329463186 1.2159259657270991 100.41941012176017 ;
createNode camera -s -n "frontShape" -p "front";
	setAttr -k off ".v" no;
	setAttr ".rnd" no;
	setAttr ".coi" 100.1;
	setAttr ".ow" 28.854732146361307;
	setAttr ".imn" -type "string" "front";
	setAttr ".den" -type "string" "front_depth";
	setAttr ".man" -type "string" "front_mask";
	setAttr ".hc" -type "string" "viewSet -f %camera";
	setAttr ".o" yes;
createNode transform -s -n "side";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 100.15670934124184 2.0787389182130616 -5.9604623309499015e-008 ;
	setAttr ".r" -type "double3" 0 89.999999999999986 0 ;
createNode camera -s -n "sideShape" -p "side";
	setAttr -k off ".v" no;
	setAttr ".rnd" no;
	setAttr ".coi" 100.1;
	setAttr ".ow" 7.2423454345032505;
	setAttr ".imn" -type "string" "side";
	setAttr ".den" -type "string" "side_depth";
	setAttr ".man" -type "string" "side_mask";
	setAttr ".hc" -type "string" "viewSet -s %camera";
	setAttr ".o" yes;
createNode transform -n "particle1";
createNode particle -n "particleShape1" -p "particle1";
	addAttr -ci true -sn "lifespanPP" -ln "lifespanPP" -bt "life" -dt "doubleArray";
	addAttr -ci true -h true -sn "lifespanPP0" -ln "lifespanPP0" -bt "life" -dt "doubleArray";
	addAttr -ci true -sn "lifespan" -ln "lifespan" -bt "life" -at "double";
	addAttr -is true -ci true -sn "radius" -ln "radius" -dv 0.5 -min 0 -max 10 -at "float";
	addAttr -is true -ci true -sn "opacity" -ln "opacity" -at "double";
	addAttr -is true -ci true -sn "colorRed" -ln "colorRed" -at "double";
	addAttr -is true -ci true -sn "colorGreen" -ln "colorGreen" -at "double";
	addAttr -is true -ci true -sn "colorBlue" -ln "colorBlue" -at "double";
	addAttr -is true -ci true -sn "spriteTwist" -ln "spriteTwist" -min -180 -max 180 
		-at "float";
	addAttr -is true -ci true -sn "spriteScaleX" -ln "spriteScaleX" -dv 1 -at "double";
	addAttr -is true -ci true -sn "spriteScaleY" -ln "spriteScaleY" -dv 1 -at "double";
	addAttr -is true -ci true -sn "spriteNum" -ln "spriteNum" -dv 1 -at "long";
	addAttr -is true -ci true -sn "useLighting" -ln "useLighting" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "__ApplySprite__" -ln "__ApplySprite__" -at "message";
	addAttr -ci true -k true -sn "SpriteAnimation" -ln "SpriteAnimation" -min 0 -max 
		1 -en "off:on" -at "enum";
	addAttr -ci true -k true -sn "SpriteStartOption" -ln "SpriteStartOption" -min 0 
		-max 4 -en "first:particleId:random:custom:ramp" -at "enum";
	addAttr -ci true -k true -sn "SpriteCyclePattern" -ln "SpriteCyclePattern" -min 
		0 -max 5 -en "linearUp:easeUp:linearUpDown:easeUpDown:custom:ramp" -at "enum";
	addAttr -ci true -k true -sn "SpriteCycleLength" -ln "SpriteCycleLength" -min 0 
		-at "double";
	addAttr -ci true -sn "SpriteInvertCycle" -ln "SpriteInvertCycle" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "ageNormalized" -ln "ageNormalized" -dt "doubleArray";
	addAttr -ci true -h true -sn "ageNormalized0" -ln "ageNormalized0" -dt "doubleArray";
	addAttr -ci true -sn "spriteNumPP" -ln "spriteNumPP" -dt "doubleArray";
	addAttr -ci true -h true -sn "spriteNumPP0" -ln "spriteNumPP0" -dt "doubleArray";
	addAttr -ci true -sn "startIndexPP" -ln "startIndexPP" -dt "doubleArray";
	addAttr -ci true -h true -sn "startIndexPP0" -ln "startIndexPP0" -dt "doubleArray";
	addAttr -ci true -sn "spriteNumRamp" -ln "spriteNumRamp" -dt "doubleArray";
	addAttr -ci true -h true -sn "spriteNumRamp0" -ln "spriteNumRamp0" -dt "doubleArray";
	addAttr -ci true -sn "spriteNumRampU" -ln "spriteNumRampU" -dt "doubleArray";
	addAttr -ci true -h true -sn "spriteNumRampU0" -ln "spriteNumRampU0" -dt "doubleArray";
	setAttr -k off ".v";
	setAttr ".gf" -type "Int32Array" 0 ;
	setAttr ".pos0" -type "vectorArray" 0 ;
	setAttr ".vel0" -type "vectorArray" 0 ;
	setAttr ".acc0" -type "vectorArray" 0 ;
	setAttr ".usc" yes;
	setAttr ".scp" -type "string" "test_particles_startup";
	setAttr ".mas0" -type "doubleArray" 0 ;
	setAttr ".id0" -type "doubleArray" 0 ;
	setAttr ".bt0" -type "doubleArray" 0 ;
	setAttr ".ag0" -type "doubleArray" 0 ;
	setAttr -s 2 ".npt";
	setAttr ".lfm" 2;
	setAttr ".lfr" 2;
	setAttr -s 11 ".xi";
	setAttr -s 2 ".xo";
	setAttr ".irbx" -type "string" (
		"// __ApplySprite_Start_Tag__\n//\n// Do not delete the tag above.  It is used to properly\n// replace this expression if the applySprite() script\n// is executed again.\n//\nint $firstImageNumber = .I[0];\nint $lastImageNumber = .I[1];\nint $imageCount = $lastImageNumber - $firstImageNumber + 1;\nint $one_based_start = 1;\n\nif( .I[2] == 1 )\n{\n\t// If particles live forever, the animation option won't work.\n\t// Switch them to constant so that the user can see their animation.\n\t//\n\tif (.O[0] == 0) {\n\t\t.O[0] = 1;\n\t}\n\n\tif ( .I[3] < 4 )\n\t{\n\t\t//\n\t\t// Find out how far into the cycle function to offset the lookup, based\n\t\t// on the startIndexPP of this particle.\n\t\t//\n\t\tfloat $startIndexPPRatio = 0;\n\t\t{\n\t\t\t$startIndexPPRatio = ( .O[1] - $one_based_start ) / ( $imageCount );\n\n\t\t\t//\n\t\t\t// If the cycle function starts off moving in the negative direction, we move the offset\n\t\t\t// forward a little bit to allow the first value to not immediately change as its value\n\t\t\t// crosses down to the next smaller integer.\n\t\t\t//\n\t\t\tif( .I[4] == 1 )\n"
		+ "\t\t\t{\n\t\t\t\t$startIndexPPRatio = ( .O[1] + 1 - $one_based_start ) / ( $imageCount );\n\t\t\t}\n\n\t\t\t//\n\t\t\t// Adjust the offset into the cycle function so that it starts at the same place\n\t\t\t// as the startIndexPP would be in the function.\n\t\t\t//\n\t\t\tif( .I[3] == 0 )\n\t\t\t{\n\t\t\t\t$startIndexPPRatio = $startIndexPPRatio;\n\t\t\t}\n\t\t\telse if( .I[3] == 1 )\n\t\t\t{\n\t\t\t\tfloat $ts = ( ( $startIndexPPRatio * 2.0 ) - 1.0 );\n\t\t\t\tfloat $ac = acos( $ts );\n\t\t\t\t$nv = ( 3.14159 - $ac ) / 3.14159;\n\t\t\t\t$startIndexPPRatio = $nv;\n\t\t\t}\n\t\t\telse if( .I[3] == 2 )\n\t\t\t{\n\t\t\t\t$startIndexPPRatio = $startIndexPPRatio / 2.0;\n\t\t\t}\n\t\t\telse if( .I[3] == 3 )\n\t\t\t{\n\t\t\t\tfloat $ts = ( ( $startIndexPPRatio * 2.0 ) - 1.0 );\n\t\t\t\tfloat $ac = acos( $ts );\n\t\t\t\t$nv = ( 3.14159 - $ac ) / 3.14159;\n\t\t\t\t$startIndexPPRatio = $nv / 2.0;\n\t\t\t}\n\t\t}\n\n\t\t//\n\t\t// Compute the value to use to access the curve function for the given cycle option.\n\t\t//\n\t\tfloat $a = 0;\n\t\t{\n\t\t\tif( .I[5] <= 0 )\n\t\t\t{\n\t\t\t\tif( .I[4] == 0 )\n\t\t\t\t{\n\t\t\t\t\t$a = ( .I[6] + $startIndexPPRatio ) % 1.0;\n\t\t\t\t}\n\t\t\t\telse\n\t\t\t\t{\n\t\t\t\t\t$a = ( $startIndexPPRatio - .I[6] );\n"
		+ "\t\t\t\t\twhile( $a < 0 )\n\t\t\t\t\t{\n\t\t\t\t\t\t$a = $a + 1.0;\n\t\t\t\t\t}\n\t\t\t\t}\n\t\t\t}\n\t\t\telse\n\t\t\t{\n\t\t\t\tfloat $fps = 30.0;\n\t\t\t\tstring $timeUnit = `currentUnit -q -time`;\n\t\t\t\tif( $timeUnit == \"film\")\n\t\t\t\t\t$fps = 24.0;\n\t\t\t\telse if( $timeUnit == \"ntsc\")\n\t\t\t\t\t$fps = 30.0;\n\t\t\t\telse if( $timeUnit == \"pal\")\n\t\t\t\t\t$fps = 25.0;\n\t\t\t\telse if( $timeUnit == \"game\")\n\t\t\t\t\t$fps = 15.0;\n\t\t\t\telse if( $timeUnit == \"palf\")\n\t\t\t\t\t$fps = 50.0;\n\t\t\t\telse if( $timeUnit == \"ntscf\")\n\t\t\t\t\t$fsp = 60.0;\n\n\t\t\t\tfloat $age = .I[7];\n\t\t\t\tfloat $ageFrames = $age * $fps;\n\t\t\t\tif( .I[4] == 0 )\n\t\t\t\t{\n\t\t\t\t\t$a = ( ( $ageFrames / .I[5] ) + $startIndexPPRatio ) % 1.0;\n\t\t\t\t}\n\t\t\t\telse\n\t\t\t\t{\n\t\t\t\t\t$a = ( $startIndexPPRatio - ( $ageFrames / .I[5] ) );\n\t\t\t\t\twhile( $a < 0 )\n\t\t\t\t\t{\n\t\t\t\t\t\t$a = $a + 1.0;\n\t\t\t\t\t}\n\t\t\t\t}\n\t\t\t}\n\n\t\t\tif( ( .I[3] == 2 ) ||\n\t\t\t    ( .I[3] == 3 ) )\n\t\t\t{\n\t\t\t\t$a = $a * 2.0;\n\t\t\t}\n\t\t}\n\n\t\t//\n\t\t// Compute value from $a and the curve representing the desired cycle option.\n\t\t//\n\t\tfloat $v = 0;\n\t\t{\n\t\t\t//\n\t\t\t// Linearly increasing.\n\t\t\t//\n\t\t\tif( .I[3] == 0 )\n\t\t\t{\n\t\t\t\t$v = 1.0 - abs( 1.0 -$a );\n"
		+ "\t\t\t}\n\t\t\t//\n\t\t\t// Linearly decreasing.\n\t\t\t//\n\t\t\telse if( .I[3] == 1 )\n\t\t\t{\n\t\t\t\tfloat $ta = ( $a + 1.0 ) * 3.14159;\n\t\t\t\tfloat $nv = ( cos( $ta ) + 1.0 ) / 2.0;\n\t\t\t\t$v = $nv;\n\t\t\t}\n\t\t\t//\n\t\t\t// Linearly increasing, then decreasing.\n\t\t\t//\n\t\t\telse if( .I[3] == 2 )\n\t\t\t{\n\t\t\t\t$v = 1.0 - abs( 1.0 -$a );\n\t\t\t}\n\t\t\t//\n\t\t\t// Linearly decreasing, then increasing.\n\t\t\t//\n\t\t\telse if( .I[3] == 3 )\n\t\t\t{\n\t\t\t\tfloat $ta = ( $a + 1.0 ) * 3.14159;\n\t\t\t\tfloat $nv = ( cos( $ta ) + 1.0 ) / 2.0;\n\t\t\t\t$v = $nv;\n\t\t\t}\n\t\t}\n\n\t\t//\n\t\t// Now scale $v by the number of images in the set to\n\t\t// get the index.  Add one to this since the images\n\t\t// for the sprite rendering are considered to be\n\t\t// one-based.  We actually scale by just under the\n\t\t// image count.  Since this is converted to an integer,\n\t\t// this gives the last image equal time in the cycle.\n\t\t//\n\t\tint $finalIndex = $one_based_start + ( $v * ( $imageCount - 0.01 ) );\n\n\t\t//\n\t\t// Assign the index.\n\t\t//\n\t\t.O[2] = $finalIndex;\n\t}\n \telse if( .I[3] == 4 )\n \t{\n \t\t//\n \t\t// Custom Cycle\n \t\t// Put your own expression for spriteNumPP here.\n"
		+ " \t\t//\n \t}\n\telse if( .I[3] == 5 )\n\t{\n\t\tfloat $rampValue = .I[8];\n\t\tif( .I[4] == 1 )\n\t\t{\n\t\t\t$rampValue = 1.0 - $rampValue;\n\t\t\twhile( $rampValue < 0 )\n\t\t\t{\n\t\t\t\t$rampValue = $rampValue + 1.0;\n\t\t\t}\n\t\t\tif( $rampValue > 1.0 )\n\t\t\t{\n\t\t\t\t$rampValue = $rampValue % 1.0;\n\t\t\t}\n\t\t}\n\t\tint $s = $one_based_start + ( $rampValue * ( $imageCount - 0.01 ) );\n\t\tif ( .I[9] != 4)\n\t\t{\n\t\t\t// offset the particles by the start frame\n\t\t\t//\n\t\t\t$s += ( .O[1] - $one_based_start );\n\t\t\tif ($s >= $imageCount ) $s -= $imageCount;\n\t\t}\n\t\t.O[2] = $s;\n\t}\n}\n\n//\n// Do not delete the tag below.  It is used to properly\n// replace this expression if the applySprite() script\n// is executed again.\n//\n// __ApplySprite_End_Tag__");
	setAttr ".irax" -type "string" "";
	setAttr ".icx" -type "string" (
		"// __ApplySprite_Start_Tag__\n//\n// Do not delete the tag above.  It is used to properly\n// replace this expression if the applySprite() script\n// is executed again.\n//\nint $firstImageNumber = .I[0];\nint $lastImageNumber = .I[1];\nint $imageCount = $lastImageNumber - $firstImageNumber + 1;\nint $one_based_start = 1;\n\nif( .I[2] == 0 )\n//\n// Fixed SpriteNumPP\n//\n{\n\t//\n\t// Use the first image in the sequence\n\t//\n\tif( .I[9] == 0 )\n\t{\n\t\t.O[2] = $one_based_start;\n\t}\n\t//\n\t// Use the particle's ID value ( MOD imageCount )\n\t//\n\telse if( .I[9] == 1 )\n\t{\n\t\t.O[2] = $one_based_start + ( .I[10] % $imageCount );\n\t}\n\t//\n\t// Random\n\t//\n\telse if( .I[9] == 2 )\n\t{\n\t\tint $s = rand( $imageCount + 0.99 ) + $one_based_start;\n\t\t.O[2] = $s;\n\t}\n\telse if( .I[9] == 3 )\n\t//\n\t// Custom Start: If your sprites are not animated\n\t//\n\t{\n\t\t//\n\t\t// Put your own expressions to set spriteNumPP here.\n\t\t//\n\t\t.O[2] = 1;\n\t}\n\t//\n\t// Ramp, spriteNumRamp\n\t//\n\telse if( .I[9] == 4 )\n\t{\n\t\tfloat $rampValue = .I[8];\n\t\tif( .I[4] == 1 )\n\t\t{\n\t\t\t$rampValue = 1.0 - $rampValue;\n"
		+ "\t\t\twhile( $rampValue < 0 )\n\t\t\t{\n\t\t\t\t$rampValue = $rampValue + 1.0;\n\t\t\t}\n\t\t\tif( $rampValue > 1.0 )\n\t\t\t{\n\t\t\t\t$rampValue = $rampValue % 1.0;\n\t\t\t}\n\t\t}\n\t\tint $s = $one_based_start + ( $rampValue * ( $imageCount - 0.01 ) );\n\t\t.O[2] = $s;\n\t}\n}\nelse if( .I[2] == 1 )\n//\n// Cycle SpriteNumPP\n//\n{\n\t//\n\t// Use the first/last image in the sequence\n\t//\n\tif( .I[9] == 0 )\n\t{\n\t\tif( .I[4] == 1 )\n\t\t{\n\t\t\t.O[2] = $imageCount;\n\t\t}\n\t\telse\n\t\t{\n\t\t\t.O[2] = $one_based_start;\n\t\t}\n\t}\n\t//\n\t// Use the particle's ID value ( MOD imageCount )\n\t//\n\telse if( .I[9] == 1 )\n\t{\n\t\t.O[2] = $one_based_start + ( .I[10] % $imageCount );\n\t}\n\t//\n\t// Random\n\t//\n\telse if( .I[9] == 2 )\n\t{\n\t\tint $s = rand( $imageCount - 0.01 ) + $one_based_start;\n\t\t.O[2] = $s;\n\t}\n\t//\n\t// Custom Start: if your sprites are animated\n\t//\n\telse if( .I[9] == 3 )\n\t{\n\t\t//\n\t\t// Put your own expressions to set spriteNumPP here .\n\t\t//\n\t\t.O[2] = 1;\n\t}\n\t//\n\t// Ramp, spriteNumRamp\n\t//\n\telse if( .I[9] == 4 )\n\t{\n\t\tfloat $rampValue = .I[8];\n\t\tif( .I[4] == 1 )\n\t\t{\n\t\t\t$rampValue = 1.0 - $rampValue;\n"
		+ "\t\t\twhile( $rampValue < 0 )\n\t\t\t{\n\t\t\t\t$rampValue = $rampValue + 1.0;\n\t\t\t}\n\t\t\tif( $rampValue > 1.0 )\n\t\t\t{\n\t\t\t\t$rampValue = $rampValue % 1.0;\n\t\t\t}\n\t\t}\n\t\tint $s = $one_based_start + ( $rampValue * ( $imageCount - 0.01 ) );\n\t\t.O[2] = $s;\n\t}\n}\n\n//\n// Copy the spriteIndexPP value into startNumPP so that we can use that\n// value in the runtime expression.\n//\n.O[1] = .O[2];\n\n//\n// Do not delete the tag below.  It is used to properly\n// replace this expression if the applySprite() script\n// is executed again.\n//\n// __ApplySprite_End_Tag__");
	setAttr -s 2 ".sd";
	setAttr -s 2 ".sd";
	setAttr ".cts" 1;
	setAttr ".chw" 100;
	setAttr ".ds" yes;
	setAttr ".prt" 5;
	setAttr ".lifespanPP0" -type "doubleArray" 0 ;
	setAttr -k on ".lifespan" 3;
	setAttr ".radius" 0.20000000298023224;
	setAttr -k on ".opacity" 1;
	setAttr -k on ".colorRed" 1;
	setAttr -k on ".colorGreen" 1;
	setAttr -k on ".colorBlue";
	setAttr -k on ".spriteTwist";
	setAttr -k on ".spriteScaleX" 3;
	setAttr -k on ".spriteScaleY" 3;
	setAttr -k on ".spriteNum";
	setAttr -k on ".useLighting";
	setAttr -k on ".SpriteAnimation";
	setAttr -k on ".SpriteStartOption" 2;
	setAttr -k on ".SpriteCyclePattern";
	setAttr -k on ".SpriteCycleLength";
	setAttr ".ageNormalized0" -type "doubleArray" 0 ;
	setAttr ".spriteNumPP0" -type "doubleArray" 0 ;
	setAttr ".startIndexPP0" -type "doubleArray" 0 ;
	setAttr ".spriteNumRamp0" -type "doubleArray" 0 ;
	setAttr ".spriteNumRampU0" -type "doubleArray" 0 ;
createNode transform -n "emitting_cylinder";
	setAttr ".smd" 7;
createNode pointEmitter -n "emitter1" -p "emitting_cylinder";
	setAttr ".emt" 0;
	setAttr ".sro" no;
	setAttr -l on ".urpp";
	setAttr ".d" -type "double3" 0 0 1 ;
	setAttr ".spr" 0.1;
createNode transform -n "pCylinder1" -p "emitting_cylinder";
	setAttr ".t" -type "double3" -1.439 0 0 ;
	setAttr ".r" -type "double3" -90 -6.361109362927032e-015 89.999999999999943 ;
	setAttr ".s" -type "double3" 0.5 1.443180711778383 0.5 ;
	setAttr ".rp" -type "double3" 3.1952218648711987e-016 -1.439 0 ;
	setAttr ".rpt" -type "double3" 1.4389999999999998 1.439 0 ;
	setAttr ".sp" -type "double3" 6.3904437297423934e-016 -0.99710312662560974 0 ;
	setAttr ".spt" -type "double3" -3.1952218648711967e-016 -0.44189687337439021 0 ;
createNode mesh -n "pCylinderShape1" -p "pCylinder1";
	addAttr -ci true -sn "mso" -ln "miShadingSamplesOverride" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "msh" -ln "miShadingSamples" -min 0 -smx 8 -at "float";
	addAttr -ci true -sn "mdo" -ln "miMaxDisplaceOverride" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "mmd" -ln "miMaxDisplace" -min 0 -smx 1 -at "float";
	setAttr -k off ".v";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr ".pt[41]" -type "float3"  8.0611993e-017 0.95296252 5.8280118e-016;
createNode pointEmitter -n "emitter2";
	setAttr ".sro" no;
	setAttr -l on ".urpp";
createNode transform -n "curve1";
createNode nurbsCurve -n "curveShape1" -p "curve1";
	setAttr -k off ".v";
	setAttr ".cc" -type "nurbsCurve" 
		3 14 2 no 3
		19 -0.10000000000000001 -0.050000000000000003 0 1 2 3 4 5 6 7 8 9 10 11 11.050000000000001
		 11.100000000000001 11.150000000000002 12.150000000000002 13.150000000000002
		17
		5.1205575801978878 -0.050649926493961377 0
		5.3452268597743604 0.58630591240623309 0
		7.4454835976233724 3.4549148744473905 0
		7.7490548263295 8.2856909682488524 1.7504473771227445
		0.58393467262517662 7.4632338495163806 -4.5071914106336344
		-1.1108388929500741 -0.79138762026412901 0
		-6.0288510104499018 -5.6339939538359429 -1.8572057151739481
		-9.9182306044850712 -3.584847534184143 0
		-11.501914192401339 6.1688220677939274 0
		-3.8353902851421067 7.9446303841218091 2.3524605725536674
		-0.94792684574527519 1.3180876705977984 0
		1.2555120613541297 -4.3229314981241904 2.8639714402388421
		3.9642590050881377 -1.6344126962759173 0
		4.6864764971550841 -0.92304932912800919 0
		5.1205575801978878 -0.050649926493961377 0
		5.3452268597743604 0.58630591240623309 0
		7.4454835976233724 3.4549148744473905 0
		;
createNode transform -n "positionMarker1" -p "curveShape1";
createNode positionMarker -n "positionMarkerShape1" -p "positionMarker1";
	setAttr -k off ".v";
	setAttr ".uwo" yes;
	setAttr ".t" 1;
createNode transform -n "positionMarker2" -p "curveShape1";
createNode positionMarker -n "positionMarkerShape2" -p "positionMarker2";
	setAttr -k off ".v";
	setAttr ".uwo" yes;
	setAttr ".lp" -type "double3" 1 0 0 ;
	setAttr ".t" 100;
createNode pointEmitter -n "emitter3";
	setAttr ".t" -type "double3" 0 0 -6 ;
	setAttr ".r" -type "double3" -10 0 0 ;
	setAttr ".emt" 0;
	setAttr ".sro" no;
	setAttr -l on ".urpp";
	setAttr ".d" -type "double3" 0 1 0 ;
	setAttr ".spr" 0.2;
	setAttr ".spd" 10;
createNode transform -n "particle2";
createNode particle -n "particleShape2" -p "particle2";
	addAttr -ci true -sn "lifespanPP" -ln "lifespanPP" -bt "life" -dt "doubleArray";
	addAttr -ci true -h true -sn "lifespanPP0" -ln "lifespanPP0" -bt "life" -dt "doubleArray";
	addAttr -ci true -sn "lifespan" -ln "lifespan" -bt "life" -at "double";
	addAttr -is true -ci true -sn "radius" -ln "radius" -dv 0.5 -min 0 -max 10 -at "float";
	addAttr -is true -ci true -sn "colorAccum" -ln "colorAccum" -min 0 -max 1 -at "bool";
	addAttr -is true -ci true -sn "useLighting" -ln "useLighting" -min 0 -max 1 -at "bool";
	addAttr -is true -ci true -sn "lineWidth" -ln "lineWidth" -dv 1 -min 1 -max 20 -at "long";
	addAttr -is true -ci true -sn "tailFade" -ln "tailFade" -min -1 -max 1 -at "float";
	addAttr -is true -ci true -sn "tailSize" -ln "tailSize" -dv 1 -min -100 -max 100 
		-at "float";
	addAttr -is true -ci true -sn "normalDir" -ln "normalDir" -dv 2 -min 1 -max 3 -at "long";
	addAttr -is true -ci true -sn "opacity" -ln "opacity" -at "double";
	addAttr -is true -ci true -sn "colorRed" -ln "colorRed" -at "double";
	addAttr -is true -ci true -sn "colorGreen" -ln "colorGreen" -at "double";
	addAttr -is true -ci true -sn "colorBlue" -ln "colorBlue" -at "double";
	setAttr -k off ".v";
	setAttr ".gf" -type "Int32Array" 0 ;
	setAttr ".pos0" -type "vectorArray" 0 ;
	setAttr ".vel0" -type "vectorArray" 0 ;
	setAttr ".acc0" -type "vectorArray" 0 ;
	setAttr ".usc" yes;
	setAttr ".scp" -type "string" "test_particles_startup";
	setAttr ".mas0" -type "doubleArray" 0 ;
	setAttr ".id0" -type "doubleArray" 0 ;
	setAttr ".bt0" -type "doubleArray" 0 ;
	setAttr ".ag0" -type "doubleArray" 0 ;
	setAttr -s 2 ".npt";
	setAttr ".lfm" 2;
	setAttr ".irbx" -type "string" "";
	setAttr ".irax" -type "string" "";
	setAttr ".icx" -type "string" "";
	setAttr -s 2 ".sd";
	setAttr -s 2 ".sd";
	setAttr ".cts" 1;
	setAttr ".chw" 100;
	setAttr ".prt" 6;
	setAttr ".lifespanPP0" -type "doubleArray" 0 ;
	setAttr -k on ".lifespan" 1;
	setAttr -k on ".colorAccum";
	setAttr -k on ".useLighting";
	setAttr -k on ".lineWidth";
	setAttr -k on ".tailFade";
	setAttr -k on ".tailSize";
	setAttr -k on ".normalDir";
	setAttr -k on ".opacity" 1;
	setAttr -k on ".colorRed";
	setAttr -k on ".colorGreen" 1;
	setAttr -k on ".colorBlue";
createNode pointEmitter -n "emitter4";
	setAttr ".t" -type "double3" 0 0 6 ;
	setAttr ".r" -type "double3" 10 0 0 ;
	setAttr ".emt" 0;
	setAttr ".sro" no;
	setAttr -l on ".urpp";
	setAttr ".d" -type "double3" 0 1 0 ;
	setAttr ".spr" 0.2;
	setAttr ".spd" 10;
createNode lightLinker -n "lightLinker1";
	setAttr -s 4 ".lnk";
	setAttr -s 4 ".slnk";
createNode displayLayerManager -n "layerManager";
createNode displayLayer -n "defaultLayer";
createNode renderLayerManager -n "renderLayerManager";
createNode renderLayer -n "defaultRenderLayer";
	setAttr ".g" yes;
createNode script -n "uiConfigurationScriptNode";
	setAttr ".b" -type "string" (
		"// Maya Mel UI Configuration File.\n//\n//  This script is machine generated.  Edit at your own risk.\n//\n//\n\nglobal string $gMainPane;\nif (`paneLayout -exists $gMainPane`) {\n\n\tglobal int $gUseScenePanelConfig;\n\tint    $useSceneConfig = $gUseScenePanelConfig;\n\tint    $menusOkayInPanels = `optionVar -q allowMenusInPanels`;\tint    $nVisPanes = `paneLayout -q -nvp $gMainPane`;\n\tint    $nPanes = 0;\n\tstring $editorName;\n\tstring $panelName;\n\tstring $itemFilterName;\n\tstring $panelConfig;\n\n\t//\n\t//  get current state of the UI\n\t//\n\tsceneUIReplacement -update $gMainPane;\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Top View\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `modelPanel -unParent -l (localizedPanelLabel(\"Top View\")) -mbv $menusOkayInPanels `;\n\t\t\t$editorName = $panelName;\n            modelEditor -e \n                -camera \"top\" \n                -useInteractiveMode 0\n                -displayLights \"default\" \n                -displayAppearance \"wireframe\" \n"
		+ "                -activeOnly 0\n                -wireframeOnShaded 0\n                -headsUpDisplay 1\n                -selectionHiliteDisplay 1\n                -useDefaultMaterial 0\n                -bufferMode \"double\" \n                -twoSidedLighting 1\n                -backfaceCulling 0\n                -xray 0\n                -jointXray 0\n                -activeComponentsXray 0\n                -displayTextures 0\n                -smoothWireframe 0\n                -lineWidth 1\n                -textureAnisotropic 0\n                -textureHilight 1\n                -textureSampling 2\n                -textureDisplay \"modulate\" \n                -textureMaxSize 8192\n                -fogging 0\n                -fogSource \"fragment\" \n                -fogMode \"linear\" \n                -fogStart 0\n                -fogEnd 100\n                -fogDensity 0.1\n                -fogColor 0.5 0.5 0.5 1 \n                -maxConstantTransparency 1\n                -rendererName \"base_OpenGL_Renderer\" \n                -colorResolution 256 256 \n"
		+ "                -bumpResolution 512 512 \n                -textureCompression 0\n                -transparencyAlgorithm \"frontAndBackCull\" \n                -transpInShadows 0\n                -cullingOverride \"none\" \n                -lowQualityLighting 0\n                -maximumNumHardwareLights 1\n                -occlusionCulling 0\n                -shadingModel 0\n                -useBaseRenderer 0\n                -useReducedRenderer 0\n                -smallObjectCulling 0\n                -smallObjectThreshold -1 \n                -interactiveDisableShadows 0\n                -interactiveBackFaceCull 0\n                -sortTransparent 1\n                -nurbsCurves 1\n                -nurbsSurfaces 1\n                -polymeshes 1\n                -subdivSurfaces 1\n                -planes 1\n                -lights 1\n                -cameras 1\n                -controlVertices 1\n                -hulls 1\n                -grid 1\n                -joints 1\n                -ikHandles 1\n                -deformers 1\n                -dynamics 1\n"
		+ "                -fluids 1\n                -hairSystems 1\n                -follicles 1\n                -nCloths 1\n                -nParticles 1\n                -nRigids 1\n                -dynamicConstraints 1\n                -locators 1\n                -manipulators 1\n                -dimensions 1\n                -handles 1\n                -pivots 1\n                -textures 1\n                -strokes 1\n                -shadows 0\n                $editorName;\nmodelEditor -e -viewSelected 0 $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Top View\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"top\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"wireframe\" \n            -activeOnly 0\n            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n"
		+ "            -twoSidedLighting 1\n            -backfaceCulling 0\n            -xray 0\n            -jointXray 0\n            -activeComponentsXray 0\n            -displayTextures 0\n            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n            -textureDisplay \"modulate\" \n            -textureMaxSize 8192\n            -fogging 0\n            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n            -maxConstantTransparency 1\n            -rendererName \"base_OpenGL_Renderer\" \n            -colorResolution 256 256 \n            -bumpResolution 512 512 \n            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n            -lowQualityLighting 0\n            -maximumNumHardwareLights 1\n            -occlusionCulling 0\n"
		+ "            -shadingModel 0\n            -useBaseRenderer 0\n            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n            -sortTransparent 1\n            -nurbsCurves 1\n            -nurbsSurfaces 1\n            -polymeshes 1\n            -subdivSurfaces 1\n            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n            -hulls 1\n            -grid 1\n            -joints 1\n            -ikHandles 1\n            -deformers 1\n            -dynamics 1\n            -fluids 1\n            -hairSystems 1\n            -follicles 1\n            -nCloths 1\n            -nParticles 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -dimensions 1\n            -handles 1\n            -pivots 1\n            -textures 1\n            -strokes 1\n            -shadows 0\n            $editorName;\nmodelEditor -e -viewSelected 0 $editorName;\n"
		+ "\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Side View\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `modelPanel -unParent -l (localizedPanelLabel(\"Side View\")) -mbv $menusOkayInPanels `;\n\t\t\t$editorName = $panelName;\n            modelEditor -e \n                -camera \"side\" \n                -useInteractiveMode 0\n                -displayLights \"default\" \n                -displayAppearance \"wireframe\" \n                -activeOnly 0\n                -wireframeOnShaded 0\n                -headsUpDisplay 1\n                -selectionHiliteDisplay 1\n                -useDefaultMaterial 0\n                -bufferMode \"double\" \n                -twoSidedLighting 1\n                -backfaceCulling 0\n                -xray 0\n                -jointXray 0\n                -activeComponentsXray 0\n                -displayTextures 0\n                -smoothWireframe 0\n                -lineWidth 1\n                -textureAnisotropic 0\n"
		+ "                -textureHilight 1\n                -textureSampling 2\n                -textureDisplay \"modulate\" \n                -textureMaxSize 8192\n                -fogging 0\n                -fogSource \"fragment\" \n                -fogMode \"linear\" \n                -fogStart 0\n                -fogEnd 100\n                -fogDensity 0.1\n                -fogColor 0.5 0.5 0.5 1 \n                -maxConstantTransparency 1\n                -rendererName \"base_OpenGL_Renderer\" \n                -colorResolution 256 256 \n                -bumpResolution 512 512 \n                -textureCompression 0\n                -transparencyAlgorithm \"frontAndBackCull\" \n                -transpInShadows 0\n                -cullingOverride \"none\" \n                -lowQualityLighting 0\n                -maximumNumHardwareLights 1\n                -occlusionCulling 0\n                -shadingModel 0\n                -useBaseRenderer 0\n                -useReducedRenderer 0\n                -smallObjectCulling 0\n                -smallObjectThreshold -1 \n"
		+ "                -interactiveDisableShadows 0\n                -interactiveBackFaceCull 0\n                -sortTransparent 1\n                -nurbsCurves 1\n                -nurbsSurfaces 1\n                -polymeshes 1\n                -subdivSurfaces 1\n                -planes 1\n                -lights 1\n                -cameras 1\n                -controlVertices 1\n                -hulls 1\n                -grid 1\n                -joints 1\n                -ikHandles 1\n                -deformers 1\n                -dynamics 1\n                -fluids 1\n                -hairSystems 1\n                -follicles 1\n                -nCloths 1\n                -nParticles 1\n                -nRigids 1\n                -dynamicConstraints 1\n                -locators 1\n                -manipulators 1\n                -dimensions 1\n                -handles 1\n                -pivots 1\n                -textures 1\n                -strokes 1\n                -shadows 0\n                $editorName;\nmodelEditor -e -viewSelected 0 $editorName;\n"
		+ "\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Side View\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"side\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"wireframe\" \n            -activeOnly 0\n            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n            -twoSidedLighting 1\n            -backfaceCulling 0\n            -xray 0\n            -jointXray 0\n            -activeComponentsXray 0\n            -displayTextures 0\n            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n            -textureDisplay \"modulate\" \n            -textureMaxSize 8192\n            -fogging 0\n            -fogSource \"fragment\" \n            -fogMode \"linear\" \n"
		+ "            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n            -maxConstantTransparency 1\n            -rendererName \"base_OpenGL_Renderer\" \n            -colorResolution 256 256 \n            -bumpResolution 512 512 \n            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n            -lowQualityLighting 0\n            -maximumNumHardwareLights 1\n            -occlusionCulling 0\n            -shadingModel 0\n            -useBaseRenderer 0\n            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n            -sortTransparent 1\n            -nurbsCurves 1\n            -nurbsSurfaces 1\n            -polymeshes 1\n            -subdivSurfaces 1\n            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n"
		+ "            -hulls 1\n            -grid 1\n            -joints 1\n            -ikHandles 1\n            -deformers 1\n            -dynamics 1\n            -fluids 1\n            -hairSystems 1\n            -follicles 1\n            -nCloths 1\n            -nParticles 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -dimensions 1\n            -handles 1\n            -pivots 1\n            -textures 1\n            -strokes 1\n            -shadows 0\n            $editorName;\nmodelEditor -e -viewSelected 0 $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Front View\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `modelPanel -unParent -l (localizedPanelLabel(\"Front View\")) -mbv $menusOkayInPanels `;\n\t\t\t$editorName = $panelName;\n            modelEditor -e \n                -camera \"front\" \n                -useInteractiveMode 0\n"
		+ "                -displayLights \"default\" \n                -displayAppearance \"wireframe\" \n                -activeOnly 0\n                -wireframeOnShaded 0\n                -headsUpDisplay 1\n                -selectionHiliteDisplay 1\n                -useDefaultMaterial 0\n                -bufferMode \"double\" \n                -twoSidedLighting 1\n                -backfaceCulling 0\n                -xray 0\n                -jointXray 0\n                -activeComponentsXray 0\n                -displayTextures 0\n                -smoothWireframe 0\n                -lineWidth 1\n                -textureAnisotropic 0\n                -textureHilight 1\n                -textureSampling 2\n                -textureDisplay \"modulate\" \n                -textureMaxSize 8192\n                -fogging 0\n                -fogSource \"fragment\" \n                -fogMode \"linear\" \n                -fogStart 0\n                -fogEnd 100\n                -fogDensity 0.1\n                -fogColor 0.5 0.5 0.5 1 \n                -maxConstantTransparency 1\n"
		+ "                -rendererName \"base_OpenGL_Renderer\" \n                -colorResolution 256 256 \n                -bumpResolution 512 512 \n                -textureCompression 0\n                -transparencyAlgorithm \"frontAndBackCull\" \n                -transpInShadows 0\n                -cullingOverride \"none\" \n                -lowQualityLighting 0\n                -maximumNumHardwareLights 1\n                -occlusionCulling 0\n                -shadingModel 0\n                -useBaseRenderer 0\n                -useReducedRenderer 0\n                -smallObjectCulling 0\n                -smallObjectThreshold -1 \n                -interactiveDisableShadows 0\n                -interactiveBackFaceCull 0\n                -sortTransparent 1\n                -nurbsCurves 1\n                -nurbsSurfaces 1\n                -polymeshes 1\n                -subdivSurfaces 1\n                -planes 1\n                -lights 1\n                -cameras 1\n                -controlVertices 1\n                -hulls 1\n                -grid 1\n"
		+ "                -joints 1\n                -ikHandles 1\n                -deformers 1\n                -dynamics 1\n                -fluids 1\n                -hairSystems 1\n                -follicles 1\n                -nCloths 1\n                -nParticles 1\n                -nRigids 1\n                -dynamicConstraints 1\n                -locators 1\n                -manipulators 1\n                -dimensions 1\n                -handles 1\n                -pivots 1\n                -textures 1\n                -strokes 1\n                -shadows 0\n                $editorName;\nmodelEditor -e -viewSelected 0 $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Front View\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"front\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"wireframe\" \n            -activeOnly 0\n            -wireframeOnShaded 0\n            -headsUpDisplay 1\n"
		+ "            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n            -twoSidedLighting 1\n            -backfaceCulling 0\n            -xray 0\n            -jointXray 0\n            -activeComponentsXray 0\n            -displayTextures 0\n            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n            -textureDisplay \"modulate\" \n            -textureMaxSize 8192\n            -fogging 0\n            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n            -maxConstantTransparency 1\n            -rendererName \"base_OpenGL_Renderer\" \n            -colorResolution 256 256 \n            -bumpResolution 512 512 \n            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n"
		+ "            -lowQualityLighting 0\n            -maximumNumHardwareLights 1\n            -occlusionCulling 0\n            -shadingModel 0\n            -useBaseRenderer 0\n            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n            -sortTransparent 1\n            -nurbsCurves 1\n            -nurbsSurfaces 1\n            -polymeshes 1\n            -subdivSurfaces 1\n            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n            -hulls 1\n            -grid 1\n            -joints 1\n            -ikHandles 1\n            -deformers 1\n            -dynamics 1\n            -fluids 1\n            -hairSystems 1\n            -follicles 1\n            -nCloths 1\n            -nParticles 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -dimensions 1\n            -handles 1\n            -pivots 1\n"
		+ "            -textures 1\n            -strokes 1\n            -shadows 0\n            $editorName;\nmodelEditor -e -viewSelected 0 $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Persp View\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `modelPanel -unParent -l (localizedPanelLabel(\"Persp View\")) -mbv $menusOkayInPanels `;\n\t\t\t$editorName = $panelName;\n            modelEditor -e \n                -camera \"persp\" \n                -useInteractiveMode 0\n                -displayLights \"default\" \n                -displayAppearance \"smoothShaded\" \n                -activeOnly 0\n                -wireframeOnShaded 0\n                -headsUpDisplay 1\n                -selectionHiliteDisplay 1\n                -useDefaultMaterial 0\n                -bufferMode \"double\" \n                -twoSidedLighting 1\n                -backfaceCulling 0\n                -xray 0\n                -jointXray 0\n"
		+ "                -activeComponentsXray 0\n                -displayTextures 1\n                -smoothWireframe 0\n                -lineWidth 1\n                -textureAnisotropic 0\n                -textureHilight 1\n                -textureSampling 2\n                -textureDisplay \"modulate\" \n                -textureMaxSize 8192\n                -fogging 0\n                -fogSource \"fragment\" \n                -fogMode \"linear\" \n                -fogStart 0\n                -fogEnd 100\n                -fogDensity 0.1\n                -fogColor 0.5 0.5 0.5 1 \n                -maxConstantTransparency 1\n                -rendererName \"base_OpenGL_Renderer\" \n                -colorResolution 256 256 \n                -bumpResolution 512 512 \n                -textureCompression 0\n                -transparencyAlgorithm \"frontAndBackCull\" \n                -transpInShadows 0\n                -cullingOverride \"none\" \n                -lowQualityLighting 0\n                -maximumNumHardwareLights 1\n                -occlusionCulling 0\n"
		+ "                -shadingModel 0\n                -useBaseRenderer 0\n                -useReducedRenderer 0\n                -smallObjectCulling 0\n                -smallObjectThreshold -1 \n                -interactiveDisableShadows 0\n                -interactiveBackFaceCull 0\n                -sortTransparent 1\n                -nurbsCurves 1\n                -nurbsSurfaces 1\n                -polymeshes 1\n                -subdivSurfaces 1\n                -planes 1\n                -lights 1\n                -cameras 1\n                -controlVertices 1\n                -hulls 1\n                -grid 1\n                -joints 1\n                -ikHandles 1\n                -deformers 1\n                -dynamics 1\n                -fluids 1\n                -hairSystems 1\n                -follicles 1\n                -nCloths 1\n                -nParticles 1\n                -nRigids 1\n                -dynamicConstraints 1\n                -locators 1\n                -manipulators 1\n                -dimensions 1\n                -handles 1\n"
		+ "                -pivots 1\n                -textures 1\n                -strokes 1\n                -shadows 0\n                $editorName;\nmodelEditor -e -viewSelected 0 $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Persp View\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"persp\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"smoothShaded\" \n            -activeOnly 0\n            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n            -twoSidedLighting 1\n            -backfaceCulling 0\n            -xray 0\n            -jointXray 0\n            -activeComponentsXray 0\n            -displayTextures 1\n            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n"
		+ "            -textureDisplay \"modulate\" \n            -textureMaxSize 8192\n            -fogging 0\n            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n            -maxConstantTransparency 1\n            -rendererName \"base_OpenGL_Renderer\" \n            -colorResolution 256 256 \n            -bumpResolution 512 512 \n            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n            -lowQualityLighting 0\n            -maximumNumHardwareLights 1\n            -occlusionCulling 0\n            -shadingModel 0\n            -useBaseRenderer 0\n            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n            -sortTransparent 1\n            -nurbsCurves 1\n            -nurbsSurfaces 1\n"
		+ "            -polymeshes 1\n            -subdivSurfaces 1\n            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n            -hulls 1\n            -grid 1\n            -joints 1\n            -ikHandles 1\n            -deformers 1\n            -dynamics 1\n            -fluids 1\n            -hairSystems 1\n            -follicles 1\n            -nCloths 1\n            -nParticles 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -dimensions 1\n            -handles 1\n            -pivots 1\n            -textures 1\n            -strokes 1\n            -shadows 0\n            $editorName;\nmodelEditor -e -viewSelected 0 $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"outlinerPanel\" (localizedPanelLabel(\"Outliner\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `outlinerPanel -unParent -l (localizedPanelLabel(\"Outliner\")) -mbv $menusOkayInPanels `;\n"
		+ "\t\t\t$editorName = $panelName;\n            outlinerEditor -e \n                -showShapes 0\n                -showAttributes 0\n                -showConnected 0\n                -showAnimCurvesOnly 0\n                -showMuteInfo 0\n                -organizeByLayer 1\n                -showAnimLayerWeight 1\n                -autoExpandLayers 1\n                -autoExpand 0\n                -showDagOnly 1\n                -showAssets 1\n                -showContainedOnly 1\n                -showPublishedAsConnected 0\n                -showContainerContents 1\n                -ignoreDagHierarchy 0\n                -expandConnections 0\n                -showUnitlessCurves 1\n                -showCompounds 1\n                -showLeafs 1\n                -showNumericAttrsOnly 0\n                -highlightActive 1\n                -autoSelectNewObjects 0\n                -doNotSelectNewObjects 0\n                -dropIsParent 1\n                -transmitFilters 0\n                -setFilter \"defaultSetFilter\" \n                -showSetMembers 1\n"
		+ "                -allowMultiSelection 1\n                -alwaysToggleSelect 0\n                -directSelect 0\n                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -containersIgnoreFilters 0\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n                -showUVAttrsOnly 0\n                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -animLayerFilterOptions \"allAffecting\" \n                -sortOrder \"none\" \n                -longNames 0\n                -niceNames 1\n                -showNamespace 1\n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\toutlinerPanel -edit -l (localizedPanelLabel(\"Outliner\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        outlinerEditor -e \n            -showShapes 0\n            -showAttributes 0\n            -showConnected 0\n            -showAnimCurvesOnly 0\n            -showMuteInfo 0\n"
		+ "            -organizeByLayer 1\n            -showAnimLayerWeight 1\n            -autoExpandLayers 1\n            -autoExpand 0\n            -showDagOnly 1\n            -showAssets 1\n            -showContainedOnly 1\n            -showPublishedAsConnected 0\n            -showContainerContents 1\n            -ignoreDagHierarchy 0\n            -expandConnections 0\n            -showUnitlessCurves 1\n            -showCompounds 1\n            -showLeafs 1\n            -showNumericAttrsOnly 0\n            -highlightActive 1\n            -autoSelectNewObjects 0\n            -doNotSelectNewObjects 0\n            -dropIsParent 1\n            -transmitFilters 0\n            -setFilter \"defaultSetFilter\" \n            -showSetMembers 1\n            -allowMultiSelection 1\n            -alwaysToggleSelect 0\n            -directSelect 0\n            -displayMode \"DAG\" \n            -expandObjects 0\n            -setsIgnoreFilters 1\n            -containersIgnoreFilters 0\n            -editAttrName 0\n            -showAttrValues 0\n            -highlightSecondary 0\n"
		+ "            -showUVAttrsOnly 0\n            -showTextureNodesOnly 0\n            -attrAlphaOrder \"default\" \n            -animLayerFilterOptions \"allAffecting\" \n            -sortOrder \"none\" \n            -longNames 0\n            -niceNames 1\n            -showNamespace 1\n            $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"graphEditor\" (localizedPanelLabel(\"Graph Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"graphEditor\" -l (localizedPanelLabel(\"Graph Editor\")) -mbv $menusOkayInPanels `;\n\n\t\t\t$editorName = ($panelName+\"OutlineEd\");\n            outlinerEditor -e \n                -showShapes 1\n                -showAttributes 1\n                -showConnected 1\n                -showAnimCurvesOnly 1\n                -showMuteInfo 0\n                -organizeByLayer 1\n                -showAnimLayerWeight 1\n                -autoExpandLayers 1\n                -autoExpand 1\n"
		+ "                -showDagOnly 0\n                -showAssets 1\n                -showContainedOnly 0\n                -showPublishedAsConnected 0\n                -showContainerContents 0\n                -ignoreDagHierarchy 0\n                -expandConnections 1\n                -showUnitlessCurves 1\n                -showCompounds 0\n                -showLeafs 1\n                -showNumericAttrsOnly 1\n                -highlightActive 0\n                -autoSelectNewObjects 1\n                -doNotSelectNewObjects 0\n                -dropIsParent 1\n                -transmitFilters 1\n                -setFilter \"0\" \n                -showSetMembers 0\n                -allowMultiSelection 1\n                -alwaysToggleSelect 0\n                -directSelect 0\n                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -containersIgnoreFilters 0\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n                -showUVAttrsOnly 0\n"
		+ "                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -animLayerFilterOptions \"allAffecting\" \n                -sortOrder \"none\" \n                -longNames 0\n                -niceNames 1\n                -showNamespace 1\n                $editorName;\n\n\t\t\t$editorName = ($panelName+\"GraphEd\");\n            animCurveEditor -e \n                -displayKeys 1\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 1\n                -displayInfinities 0\n                -autoFit 0\n                -snapTime \"integer\" \n                -snapValue \"none\" \n                -showResults \"off\" \n                -showBufferCurves \"off\" \n                -smoothness \"fine\" \n                -resultSamples 1\n                -resultScreenSamples 0\n                -resultUpdate \"delayed\" \n                -constrainDrag 0\n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Graph Editor\")) -mbv $menusOkayInPanels  $panelName;\n"
		+ "\t\t\t$editorName = ($panelName+\"OutlineEd\");\n            outlinerEditor -e \n                -showShapes 1\n                -showAttributes 1\n                -showConnected 1\n                -showAnimCurvesOnly 1\n                -showMuteInfo 0\n                -organizeByLayer 1\n                -showAnimLayerWeight 1\n                -autoExpandLayers 1\n                -autoExpand 1\n                -showDagOnly 0\n                -showAssets 1\n                -showContainedOnly 0\n                -showPublishedAsConnected 0\n                -showContainerContents 0\n                -ignoreDagHierarchy 0\n                -expandConnections 1\n                -showUnitlessCurves 1\n                -showCompounds 0\n                -showLeafs 1\n                -showNumericAttrsOnly 1\n                -highlightActive 0\n                -autoSelectNewObjects 1\n                -doNotSelectNewObjects 0\n                -dropIsParent 1\n                -transmitFilters 1\n                -setFilter \"0\" \n                -showSetMembers 0\n"
		+ "                -allowMultiSelection 1\n                -alwaysToggleSelect 0\n                -directSelect 0\n                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -containersIgnoreFilters 0\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n                -showUVAttrsOnly 0\n                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -animLayerFilterOptions \"allAffecting\" \n                -sortOrder \"none\" \n                -longNames 0\n                -niceNames 1\n                -showNamespace 1\n                $editorName;\n\n\t\t\t$editorName = ($panelName+\"GraphEd\");\n            animCurveEditor -e \n                -displayKeys 1\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 1\n                -displayInfinities 0\n                -autoFit 0\n                -snapTime \"integer\" \n                -snapValue \"none\" \n"
		+ "                -showResults \"off\" \n                -showBufferCurves \"off\" \n                -smoothness \"fine\" \n                -resultSamples 1\n                -resultScreenSamples 0\n                -resultUpdate \"delayed\" \n                -constrainDrag 0\n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"dopeSheetPanel\" (localizedPanelLabel(\"Dope Sheet\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"dopeSheetPanel\" -l (localizedPanelLabel(\"Dope Sheet\")) -mbv $menusOkayInPanels `;\n\n\t\t\t$editorName = ($panelName+\"OutlineEd\");\n            outlinerEditor -e \n                -showShapes 1\n                -showAttributes 1\n                -showConnected 1\n                -showAnimCurvesOnly 1\n                -showMuteInfo 0\n                -organizeByLayer 1\n                -showAnimLayerWeight 1\n                -autoExpandLayers 1\n                -autoExpand 0\n"
		+ "                -showDagOnly 0\n                -showAssets 1\n                -showContainedOnly 0\n                -showPublishedAsConnected 0\n                -showContainerContents 0\n                -ignoreDagHierarchy 0\n                -expandConnections 1\n                -showUnitlessCurves 0\n                -showCompounds 1\n                -showLeafs 1\n                -showNumericAttrsOnly 1\n                -highlightActive 0\n                -autoSelectNewObjects 0\n                -doNotSelectNewObjects 1\n                -dropIsParent 1\n                -transmitFilters 0\n                -setFilter \"0\" \n                -showSetMembers 0\n                -allowMultiSelection 1\n                -alwaysToggleSelect 0\n                -directSelect 0\n                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -containersIgnoreFilters 0\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n                -showUVAttrsOnly 0\n"
		+ "                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -animLayerFilterOptions \"allAffecting\" \n                -sortOrder \"none\" \n                -longNames 0\n                -niceNames 1\n                -showNamespace 1\n                $editorName;\n\n\t\t\t$editorName = ($panelName+\"DopeSheetEd\");\n            dopeSheetEditor -e \n                -displayKeys 1\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -autoFit 0\n                -snapTime \"integer\" \n                -snapValue \"none\" \n                -outliner \"dopeSheetPanel1OutlineEd\" \n                -showSummary 1\n                -showScene 0\n                -hierarchyBelow 0\n                -showTicks 1\n                -selectionWindow 0 0 0 0 \n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Dope Sheet\")) -mbv $menusOkayInPanels  $panelName;\n"
		+ "\t\t\t$editorName = ($panelName+\"OutlineEd\");\n            outlinerEditor -e \n                -showShapes 1\n                -showAttributes 1\n                -showConnected 1\n                -showAnimCurvesOnly 1\n                -showMuteInfo 0\n                -organizeByLayer 1\n                -showAnimLayerWeight 1\n                -autoExpandLayers 1\n                -autoExpand 0\n                -showDagOnly 0\n                -showAssets 1\n                -showContainedOnly 0\n                -showPublishedAsConnected 0\n                -showContainerContents 0\n                -ignoreDagHierarchy 0\n                -expandConnections 1\n                -showUnitlessCurves 0\n                -showCompounds 1\n                -showLeafs 1\n                -showNumericAttrsOnly 1\n                -highlightActive 0\n                -autoSelectNewObjects 0\n                -doNotSelectNewObjects 1\n                -dropIsParent 1\n                -transmitFilters 0\n                -setFilter \"0\" \n                -showSetMembers 0\n"
		+ "                -allowMultiSelection 1\n                -alwaysToggleSelect 0\n                -directSelect 0\n                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -containersIgnoreFilters 0\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n                -showUVAttrsOnly 0\n                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -animLayerFilterOptions \"allAffecting\" \n                -sortOrder \"none\" \n                -longNames 0\n                -niceNames 1\n                -showNamespace 1\n                $editorName;\n\n\t\t\t$editorName = ($panelName+\"DopeSheetEd\");\n            dopeSheetEditor -e \n                -displayKeys 1\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -autoFit 0\n                -snapTime \"integer\" \n                -snapValue \"none\" \n"
		+ "                -outliner \"dopeSheetPanel1OutlineEd\" \n                -showSummary 1\n                -showScene 0\n                -hierarchyBelow 0\n                -showTicks 1\n                -selectionWindow 0 0 0 0 \n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"clipEditorPanel\" (localizedPanelLabel(\"Trax Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"clipEditorPanel\" -l (localizedPanelLabel(\"Trax Editor\")) -mbv $menusOkayInPanels `;\n\n\t\t\t$editorName = clipEditorNameFromPanel($panelName);\n            clipEditor -e \n                -displayKeys 0\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -autoFit 0\n                -snapTime \"none\" \n                -snapValue \"none\" \n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n"
		+ "\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Trax Editor\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = clipEditorNameFromPanel($panelName);\n            clipEditor -e \n                -displayKeys 0\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -autoFit 0\n                -snapTime \"none\" \n                -snapValue \"none\" \n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"hyperGraphPanel\" (localizedPanelLabel(\"Hypergraph Hierarchy\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"hyperGraphPanel\" -l (localizedPanelLabel(\"Hypergraph Hierarchy\")) -mbv $menusOkayInPanels `;\n\n\t\t\t$editorName = ($panelName+\"HyperGraphEd\");\n            hyperGraph -e \n                -graphLayoutStyle \"hierarchicalLayout\" \n                -orientation \"horiz\" \n"
		+ "                -mergeConnections 0\n                -zoom 1\n                -animateTransition 0\n                -showRelationships 1\n                -showShapes 0\n                -showDeformers 0\n                -showExpressions 0\n                -showConstraints 0\n                -showUnderworld 0\n                -showInvisible 0\n                -transitionFrames 1\n                -opaqueContainers 0\n                -freeform 0\n                -imagePosition 0 0 \n                -imageScale 1\n                -imageEnabled 0\n                -graphType \"DAG\" \n                -heatMapDisplay 0\n                -updateSelection 1\n                -updateNodeAdded 1\n                -useDrawOverrideColor 0\n                -limitGraphTraversal -1\n                -range 0 0 \n                -iconSize \"smallIcons\" \n                -showCachedConnections 0\n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Hypergraph Hierarchy\")) -mbv $menusOkayInPanels  $panelName;\n"
		+ "\t\t\t$editorName = ($panelName+\"HyperGraphEd\");\n            hyperGraph -e \n                -graphLayoutStyle \"hierarchicalLayout\" \n                -orientation \"horiz\" \n                -mergeConnections 0\n                -zoom 1\n                -animateTransition 0\n                -showRelationships 1\n                -showShapes 0\n                -showDeformers 0\n                -showExpressions 0\n                -showConstraints 0\n                -showUnderworld 0\n                -showInvisible 0\n                -transitionFrames 1\n                -opaqueContainers 0\n                -freeform 0\n                -imagePosition 0 0 \n                -imageScale 1\n                -imageEnabled 0\n                -graphType \"DAG\" \n                -heatMapDisplay 0\n                -updateSelection 1\n                -updateNodeAdded 1\n                -useDrawOverrideColor 0\n                -limitGraphTraversal -1\n                -range 0 0 \n                -iconSize \"smallIcons\" \n                -showCachedConnections 0\n"
		+ "                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"hyperShadePanel\" (localizedPanelLabel(\"Hypershade\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"hyperShadePanel\" -l (localizedPanelLabel(\"Hypershade\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Hypershade\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"visorPanel\" (localizedPanelLabel(\"Visor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"visorPanel\" -l (localizedPanelLabel(\"Visor\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Visor\")) -mbv $menusOkayInPanels  $panelName;\n"
		+ "\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"polyTexturePlacementPanel\" (localizedPanelLabel(\"UV Texture Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"polyTexturePlacementPanel\" -l (localizedPanelLabel(\"UV Texture Editor\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"UV Texture Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"multiListerPanel\" (localizedPanelLabel(\"Multilister\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"multiListerPanel\" -l (localizedPanelLabel(\"Multilister\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Multilister\")) -mbv $menusOkayInPanels  $panelName;\n"
		+ "\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"renderWindowPanel\" (localizedPanelLabel(\"Render View\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"renderWindowPanel\" -l (localizedPanelLabel(\"Render View\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Render View\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"blendShapePanel\" (localizedPanelLabel(\"Blend Shape\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\tblendShapePanel -unParent -l (localizedPanelLabel(\"Blend Shape\")) -mbv $menusOkayInPanels ;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tblendShapePanel -edit -l (localizedPanelLabel(\"Blend Shape\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n"
		+ "\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"dynRelEdPanel\" (localizedPanelLabel(\"Dynamic Relationships\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"dynRelEdPanel\" -l (localizedPanelLabel(\"Dynamic Relationships\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Dynamic Relationships\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"devicePanel\" (localizedPanelLabel(\"Devices\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\tdevicePanel -unParent -l (localizedPanelLabel(\"Devices\")) -mbv $menusOkayInPanels ;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tdevicePanel -edit -l (localizedPanelLabel(\"Devices\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n"
		+ "\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"relationshipPanel\" (localizedPanelLabel(\"Relationship Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"relationshipPanel\" -l (localizedPanelLabel(\"Relationship Editor\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Relationship Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"referenceEditorPanel\" (localizedPanelLabel(\"Reference Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"referenceEditorPanel\" -l (localizedPanelLabel(\"Reference Editor\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Reference Editor\")) -mbv $menusOkayInPanels  $panelName;\n"
		+ "\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"componentEditorPanel\" (localizedPanelLabel(\"Component Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"componentEditorPanel\" -l (localizedPanelLabel(\"Component Editor\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Component Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"dynPaintScriptedPanelType\" (localizedPanelLabel(\"Paint Effects\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"dynPaintScriptedPanelType\" -l (localizedPanelLabel(\"Paint Effects\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Paint Effects\")) -mbv $menusOkayInPanels  $panelName;\n"
		+ "\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"webBrowserPanel\" (localizedPanelLabel(\"Web Browser\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"webBrowserPanel\" -l (localizedPanelLabel(\"Web Browser\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Web Browser\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"scriptEditorPanel\" (localizedPanelLabel(\"Script Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"scriptEditorPanel\" -l (localizedPanelLabel(\"Script Editor\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Script Editor\")) -mbv $menusOkayInPanels  $panelName;\n"
		+ "\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\tif ($useSceneConfig) {\n        string $configName = `getPanel -cwl (localizedPanelLabel(\"Current Layout\"))`;\n        if (\"\" != $configName) {\n\t\t\tpanelConfiguration -edit -label (localizedPanelLabel(\"Current Layout\")) \n\t\t\t\t-defaultImage \"\"\n\t\t\t\t-image \"\"\n\t\t\t\t-sc false\n\t\t\t\t-configString \"global string $gMainPane; paneLayout -e -cn \\\"vertical2\\\" -ps 1 20 100 -ps 2 80 100 $gMainPane;\"\n\t\t\t\t-removeAllPanels\n\t\t\t\t-ap false\n\t\t\t\t\t(localizedPanelLabel(\"Outliner\")) \n\t\t\t\t\t\"outlinerPanel\"\n\t\t\t\t\t\"$panelName = `outlinerPanel -unParent -l (localizedPanelLabel(\\\"Outliner\\\")) -mbv $menusOkayInPanels `;\\n$editorName = $panelName;\\noutlinerEditor -e \\n    -showShapes 0\\n    -showAttributes 0\\n    -showConnected 0\\n    -showAnimCurvesOnly 0\\n    -showMuteInfo 0\\n    -organizeByLayer 1\\n    -showAnimLayerWeight 1\\n    -autoExpandLayers 1\\n    -autoExpand 0\\n    -showDagOnly 1\\n    -showAssets 1\\n    -showContainedOnly 1\\n    -showPublishedAsConnected 0\\n    -showContainerContents 1\\n    -ignoreDagHierarchy 0\\n    -expandConnections 0\\n    -showUnitlessCurves 1\\n    -showCompounds 1\\n    -showLeafs 1\\n    -showNumericAttrsOnly 0\\n    -highlightActive 1\\n    -autoSelectNewObjects 0\\n    -doNotSelectNewObjects 0\\n    -dropIsParent 1\\n    -transmitFilters 0\\n    -setFilter \\\"defaultSetFilter\\\" \\n    -showSetMembers 1\\n    -allowMultiSelection 1\\n    -alwaysToggleSelect 0\\n    -directSelect 0\\n    -displayMode \\\"DAG\\\" \\n    -expandObjects 0\\n    -setsIgnoreFilters 1\\n    -containersIgnoreFilters 0\\n    -editAttrName 0\\n    -showAttrValues 0\\n    -highlightSecondary 0\\n    -showUVAttrsOnly 0\\n    -showTextureNodesOnly 0\\n    -attrAlphaOrder \\\"default\\\" \\n    -animLayerFilterOptions \\\"allAffecting\\\" \\n    -sortOrder \\\"none\\\" \\n    -longNames 0\\n    -niceNames 1\\n    -showNamespace 1\\n    $editorName\"\n"
		+ "\t\t\t\t\t\"outlinerPanel -edit -l (localizedPanelLabel(\\\"Outliner\\\")) -mbv $menusOkayInPanels  $panelName;\\n$editorName = $panelName;\\noutlinerEditor -e \\n    -showShapes 0\\n    -showAttributes 0\\n    -showConnected 0\\n    -showAnimCurvesOnly 0\\n    -showMuteInfo 0\\n    -organizeByLayer 1\\n    -showAnimLayerWeight 1\\n    -autoExpandLayers 1\\n    -autoExpand 0\\n    -showDagOnly 1\\n    -showAssets 1\\n    -showContainedOnly 1\\n    -showPublishedAsConnected 0\\n    -showContainerContents 1\\n    -ignoreDagHierarchy 0\\n    -expandConnections 0\\n    -showUnitlessCurves 1\\n    -showCompounds 1\\n    -showLeafs 1\\n    -showNumericAttrsOnly 0\\n    -highlightActive 1\\n    -autoSelectNewObjects 0\\n    -doNotSelectNewObjects 0\\n    -dropIsParent 1\\n    -transmitFilters 0\\n    -setFilter \\\"defaultSetFilter\\\" \\n    -showSetMembers 1\\n    -allowMultiSelection 1\\n    -alwaysToggleSelect 0\\n    -directSelect 0\\n    -displayMode \\\"DAG\\\" \\n    -expandObjects 0\\n    -setsIgnoreFilters 1\\n    -containersIgnoreFilters 0\\n    -editAttrName 0\\n    -showAttrValues 0\\n    -highlightSecondary 0\\n    -showUVAttrsOnly 0\\n    -showTextureNodesOnly 0\\n    -attrAlphaOrder \\\"default\\\" \\n    -animLayerFilterOptions \\\"allAffecting\\\" \\n    -sortOrder \\\"none\\\" \\n    -longNames 0\\n    -niceNames 1\\n    -showNamespace 1\\n    $editorName\"\n"
		+ "\t\t\t\t-ap false\n\t\t\t\t\t(localizedPanelLabel(\"Persp View\")) \n\t\t\t\t\t\"modelPanel\"\n"
		+ "\t\t\t\t\t\"$panelName = `modelPanel -unParent -l (localizedPanelLabel(\\\"Persp View\\\")) -mbv $menusOkayInPanels `;\\n$editorName = $panelName;\\nmodelEditor -e \\n    -cam `findStartUpCamera persp` \\n    -useInteractiveMode 0\\n    -displayLights \\\"default\\\" \\n    -displayAppearance \\\"smoothShaded\\\" \\n    -activeOnly 0\\n    -wireframeOnShaded 0\\n    -headsUpDisplay 1\\n    -selectionHiliteDisplay 1\\n    -useDefaultMaterial 0\\n    -bufferMode \\\"double\\\" \\n    -twoSidedLighting 1\\n    -backfaceCulling 0\\n    -xray 0\\n    -jointXray 0\\n    -activeComponentsXray 0\\n    -displayTextures 1\\n    -smoothWireframe 0\\n    -lineWidth 1\\n    -textureAnisotropic 0\\n    -textureHilight 1\\n    -textureSampling 2\\n    -textureDisplay \\\"modulate\\\" \\n    -textureMaxSize 8192\\n    -fogging 0\\n    -fogSource \\\"fragment\\\" \\n    -fogMode \\\"linear\\\" \\n    -fogStart 0\\n    -fogEnd 100\\n    -fogDensity 0.1\\n    -fogColor 0.5 0.5 0.5 1 \\n    -maxConstantTransparency 1\\n    -rendererName \\\"base_OpenGL_Renderer\\\" \\n    -colorResolution 256 256 \\n    -bumpResolution 512 512 \\n    -textureCompression 0\\n    -transparencyAlgorithm \\\"frontAndBackCull\\\" \\n    -transpInShadows 0\\n    -cullingOverride \\\"none\\\" \\n    -lowQualityLighting 0\\n    -maximumNumHardwareLights 1\\n    -occlusionCulling 0\\n    -shadingModel 0\\n    -useBaseRenderer 0\\n    -useReducedRenderer 0\\n    -smallObjectCulling 0\\n    -smallObjectThreshold -1 \\n    -interactiveDisableShadows 0\\n    -interactiveBackFaceCull 0\\n    -sortTransparent 1\\n    -nurbsCurves 1\\n    -nurbsSurfaces 1\\n    -polymeshes 1\\n    -subdivSurfaces 1\\n    -planes 1\\n    -lights 1\\n    -cameras 1\\n    -controlVertices 1\\n    -hulls 1\\n    -grid 1\\n    -joints 1\\n    -ikHandles 1\\n    -deformers 1\\n    -dynamics 1\\n    -fluids 1\\n    -hairSystems 1\\n    -follicles 1\\n    -nCloths 1\\n    -nParticles 1\\n    -nRigids 1\\n    -dynamicConstraints 1\\n    -locators 1\\n    -manipulators 1\\n    -dimensions 1\\n    -handles 1\\n    -pivots 1\\n    -textures 1\\n    -strokes 1\\n    -shadows 0\\n    $editorName;\\nmodelEditor -e -viewSelected 0 $editorName\"\n"
		+ "\t\t\t\t\t\"modelPanel -edit -l (localizedPanelLabel(\\\"Persp View\\\")) -mbv $menusOkayInPanels  $panelName;\\n$editorName = $panelName;\\nmodelEditor -e \\n    -cam `findStartUpCamera persp` \\n    -useInteractiveMode 0\\n    -displayLights \\\"default\\\" \\n    -displayAppearance \\\"smoothShaded\\\" \\n    -activeOnly 0\\n    -wireframeOnShaded 0\\n    -headsUpDisplay 1\\n    -selectionHiliteDisplay 1\\n    -useDefaultMaterial 0\\n    -bufferMode \\\"double\\\" \\n    -twoSidedLighting 1\\n    -backfaceCulling 0\\n    -xray 0\\n    -jointXray 0\\n    -activeComponentsXray 0\\n    -displayTextures 1\\n    -smoothWireframe 0\\n    -lineWidth 1\\n    -textureAnisotropic 0\\n    -textureHilight 1\\n    -textureSampling 2\\n    -textureDisplay \\\"modulate\\\" \\n    -textureMaxSize 8192\\n    -fogging 0\\n    -fogSource \\\"fragment\\\" \\n    -fogMode \\\"linear\\\" \\n    -fogStart 0\\n    -fogEnd 100\\n    -fogDensity 0.1\\n    -fogColor 0.5 0.5 0.5 1 \\n    -maxConstantTransparency 1\\n    -rendererName \\\"base_OpenGL_Renderer\\\" \\n    -colorResolution 256 256 \\n    -bumpResolution 512 512 \\n    -textureCompression 0\\n    -transparencyAlgorithm \\\"frontAndBackCull\\\" \\n    -transpInShadows 0\\n    -cullingOverride \\\"none\\\" \\n    -lowQualityLighting 0\\n    -maximumNumHardwareLights 1\\n    -occlusionCulling 0\\n    -shadingModel 0\\n    -useBaseRenderer 0\\n    -useReducedRenderer 0\\n    -smallObjectCulling 0\\n    -smallObjectThreshold -1 \\n    -interactiveDisableShadows 0\\n    -interactiveBackFaceCull 0\\n    -sortTransparent 1\\n    -nurbsCurves 1\\n    -nurbsSurfaces 1\\n    -polymeshes 1\\n    -subdivSurfaces 1\\n    -planes 1\\n    -lights 1\\n    -cameras 1\\n    -controlVertices 1\\n    -hulls 1\\n    -grid 1\\n    -joints 1\\n    -ikHandles 1\\n    -deformers 1\\n    -dynamics 1\\n    -fluids 1\\n    -hairSystems 1\\n    -follicles 1\\n    -nCloths 1\\n    -nParticles 1\\n    -nRigids 1\\n    -dynamicConstraints 1\\n    -locators 1\\n    -manipulators 1\\n    -dimensions 1\\n    -handles 1\\n    -pivots 1\\n    -textures 1\\n    -strokes 1\\n    -shadows 0\\n    $editorName;\\nmodelEditor -e -viewSelected 0 $editorName\"\n"
		+ "\t\t\t\t$configName;\n\n            setNamedPanelLayout (localizedPanelLabel(\"Current Layout\"));\n        }\n\n        panelHistory -e -clear mainPanelHistory;\n        setFocus `paneLayout -q -p1 $gMainPane`;\n        sceneUIReplacement -deleteRemaining;\n        sceneUIReplacement -clear;\n\t}\n\n\ngrid -spacing 5 -size 12 -divisions 5 -displayAxes yes -displayGridLines yes -displayDivisionLines yes -displayPerspectiveLabels no -displayOrthographicLabels no -displayAxesBold yes -perspectiveLabelPosition axis -orthographicLabelPosition edge;\nviewManip -drawCompass 0 -compassAngle 0 -frontParameters \"\" -homeParameters \"\" -selectionLockParameters \"\";\n}\n");
	setAttr ".st" 3;
createNode script -n "sceneConfigurationScriptNode";
	setAttr ".b" -type "string" "playbackOptions -min 1 -max 100 -ast 1 -aet 100 ";
	setAttr ".st" 6;
createNode polyCylinder -n "polyCylinder1";
	setAttr ".sc" 1;
	setAttr ".cuv" 3;
createNode lambert -n "mat_rocket";
	setAttr ".c" -type "float3" 1 0.19878399 0.14399999 ;
createNode shadingEngine -n "lambert2SG";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode materialInfo -n "materialInfo1";
createNode motionPath -n "motionPath1";
	setAttr -s 2 ".pmt";
	setAttr -s 2 ".pmt";
	setAttr ".f" yes;
	setAttr ".if" yes;
	setAttr ".fa" 2;
	setAttr ".ua" 1;
	setAttr ".fm" yes;
createNode animCurveTL -n "motionPath1_uValue";
	setAttr ".tan" 10;
	setAttr ".wgt" no;
	setAttr -s 2 ".ktv[0:1]"  1 0 100 1;
createNode addDoubleLinear -n "addDoubleLinear1";
createNode addDoubleLinear -n "addDoubleLinear2";
createNode addDoubleLinear -n "addDoubleLinear3";
createNode lambert -n "lambert3";
createNode shadingEngine -n "lambert3SG";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode materialInfo -n "materialInfo2";
createNode file -n "file1";
	setAttr ".ftn" -type "string" "C:/devel/subversion/maya2osg/tests/project//sourceimages/smoke/Smoke.tga";
	setAttr ".uhc" yes;
	setAttr ".sce" 0;
	setAttr ".ece" 50;
createNode place2dTexture -n "place2dTexture1";
createNode expression -n "expression1";
	setAttr -k on ".nds";
	setAttr -s 2 ".in";
	setAttr -s 2 ".in";
	setAttr ".ixp" -type "string" "if (frame <= 1) seed(1);\n\nint $start = .I[0];\nint $end =   .I[1];\nif (frame <= $end-$start+1)\n\t.O[0] = $start + frame - 1;\nelse\n\t.O[0] = $end;";
createNode ramp -n "spriteNumRamp1";
	setAttr -s 2 ".cel";
	setAttr ".cel[0].ep" 0;
	setAttr ".cel[0].ec" -type "float3" 0 0 0 ;
	setAttr ".cel[1].ep" 1;
	setAttr ".cel[1].ec" -type "float3" 1 1 1 ;
createNode arrayMapper -n "spriteNumRampMapper1";
select -ne :time1;
	setAttr ".o" 1;
select -ne :renderPartition;
	setAttr -s 4 ".st";
select -ne :renderGlobalsList1;
select -ne :defaultShaderList1;
	setAttr -s 4 ".s";
select -ne :postProcessList1;
	setAttr -s 2 ".p";
select -ne :defaultRenderUtilityList1;
select -ne :lightList1;
select -ne :defaultTextureList1;
	setAttr -s 2 ".tx";
select -ne :initialShadingGroup;
	setAttr ".ro" yes;
select -ne :initialParticleSE;
	setAttr ".ro" yes;
select -ne :hardwareRenderGlobals;
	setAttr ".ctrs" 256;
	setAttr ".btrs" 512;
select -ne :defaultHardwareRenderGlobals;
	setAttr ".fn" -type "string" "im";
	setAttr ".res" -type "string" "ntsc_4d 646 485 1.333";
connectAttr ":time1.o" "particleShape1.cti";
connectAttr "emitter1.ot[0]" "particleShape1.npt[0]";
connectAttr "emitter2.ot[1]" "particleShape1.npt[1]";
connectAttr "spriteNumRampMapper1.ovpp" "particleShape1.spriteNumRamp";
connectAttr "file1.sce" "particleShape1.xi[0]";
connectAttr "file1.ece" "particleShape1.xi[1]";
connectAttr "particleShape1.SpriteAnimation" "particleShape1.xi[2]";
connectAttr "particleShape1.SpriteCyclePattern" "particleShape1.xi[3]";
connectAttr "particleShape1.SpriteInvertCycle" "particleShape1.xi[4]";
connectAttr "particleShape1.SpriteCycleLength" "particleShape1.xi[5]";
connectAttr "particleShape1.ageNormalized" "particleShape1.xi[6]";
connectAttr "particleShape1.ag" "particleShape1.xi[7]";
connectAttr "particleShape1.spriteNumRamp" "particleShape1.xi[8]";
connectAttr "particleShape1.SpriteStartOption" "particleShape1.xi[9]";
connectAttr "particleShape1.id" "particleShape1.xi[10]";
connectAttr "particleShape1.xo[1]" "particleShape1.startIndexPP";
connectAttr "particleShape1.xo[2]" "particleShape1.spriteNumPP";
connectAttr "addDoubleLinear1.o" "emitting_cylinder.tx";
connectAttr "addDoubleLinear2.o" "emitting_cylinder.ty";
connectAttr "addDoubleLinear3.o" "emitting_cylinder.tz";
connectAttr "motionPath1.rx" "emitting_cylinder.rx";
connectAttr "motionPath1.ry" "emitting_cylinder.ry";
connectAttr "motionPath1.rz" "emitting_cylinder.rz";
connectAttr "motionPath1.msg" "emitting_cylinder.sml";
connectAttr "motionPath1.ro" "emitting_cylinder.ro";
connectAttr ":time1.o" "emitter1.ct";
connectAttr "particleShape1.ifl" "emitter1.full[0]";
connectAttr "particleShape1.tss" "emitter1.dt[0]";
connectAttr "particleShape1.inh" "emitter1.inh[0]";
connectAttr "particleShape1.stt" "emitter1.stt[0]";
connectAttr "particleShape1.sd[0]" "emitter1.sd[0]";
connectAttr "polyCylinder1.out" "pCylinderShape1.i";
connectAttr ":time1.o" "emitter2.ct";
connectAttr "particleShape1.ifl" "emitter2.full[1]";
connectAttr "particleShape1.tss" "emitter2.dt[1]";
connectAttr "particleShape1.inh" "emitter2.inh[1]";
connectAttr "particleShape1.stt" "emitter2.stt[1]";
connectAttr "particleShape1.sd[1]" "emitter2.sd[1]";
connectAttr ":time1.o" "emitter3.ct";
connectAttr "particleShape2.ifl" "emitter3.full[0]";
connectAttr "particleShape2.tss" "emitter3.dt[0]";
connectAttr "particleShape2.inh" "emitter3.inh[0]";
connectAttr "particleShape2.stt" "emitter3.stt[0]";
connectAttr "particleShape2.sd[0]" "emitter3.sd[0]";
connectAttr ":time1.o" "particleShape2.cti";
connectAttr "emitter3.ot[0]" "particleShape2.npt[0]";
connectAttr "emitter4.ot[1]" "particleShape2.npt[1]";
connectAttr ":time1.o" "emitter4.ct";
connectAttr "particleShape2.ifl" "emitter4.full[1]";
connectAttr "particleShape2.tss" "emitter4.dt[1]";
connectAttr "particleShape2.inh" "emitter4.inh[1]";
connectAttr "particleShape2.stt" "emitter4.stt[1]";
connectAttr "particleShape2.sd[1]" "emitter4.sd[1]";
connectAttr ":defaultLightSet.msg" "lightLinker1.lnk[0].llnk";
connectAttr ":initialShadingGroup.msg" "lightLinker1.lnk[0].olnk";
connectAttr ":defaultLightSet.msg" "lightLinker1.lnk[1].llnk";
connectAttr ":initialParticleSE.msg" "lightLinker1.lnk[1].olnk";
connectAttr ":defaultLightSet.msg" "lightLinker1.lnk[2].llnk";
connectAttr "lambert2SG.msg" "lightLinker1.lnk[2].olnk";
connectAttr ":defaultLightSet.msg" "lightLinker1.lnk[3].llnk";
connectAttr "lambert3SG.msg" "lightLinker1.lnk[3].olnk";
connectAttr ":defaultLightSet.msg" "lightLinker1.slnk[0].sllk";
connectAttr ":initialShadingGroup.msg" "lightLinker1.slnk[0].solk";
connectAttr ":defaultLightSet.msg" "lightLinker1.slnk[1].sllk";
connectAttr ":initialParticleSE.msg" "lightLinker1.slnk[1].solk";
connectAttr ":defaultLightSet.msg" "lightLinker1.slnk[2].sllk";
connectAttr "lambert2SG.msg" "lightLinker1.slnk[2].solk";
connectAttr ":defaultLightSet.msg" "lightLinker1.slnk[3].sllk";
connectAttr "lambert3SG.msg" "lightLinker1.slnk[3].solk";
connectAttr "layerManager.dli[0]" "defaultLayer.id";
connectAttr "renderLayerManager.rlmi[0]" "defaultRenderLayer.rlid";
connectAttr "mat_rocket.oc" "lambert2SG.ss";
connectAttr "pCylinderShape1.iog" "lambert2SG.dsm" -na;
connectAttr "lambert2SG.msg" "materialInfo1.sg";
connectAttr "mat_rocket.msg" "materialInfo1.m";
connectAttr "motionPath1_uValue.o" "motionPath1.u";
connectAttr "curveShape1.ws" "motionPath1.gp";
connectAttr "positionMarkerShape1.t" "motionPath1.pmt[0]";
connectAttr "positionMarkerShape2.t" "motionPath1.pmt[1]";
connectAttr "emitting_cylinder.tmrx" "addDoubleLinear1.i1";
connectAttr "motionPath1.xc" "addDoubleLinear1.i2";
connectAttr "emitting_cylinder.tmry" "addDoubleLinear2.i1";
connectAttr "motionPath1.yc" "addDoubleLinear2.i2";
connectAttr "emitting_cylinder.tmrz" "addDoubleLinear3.i1";
connectAttr "motionPath1.zc" "addDoubleLinear3.i2";
connectAttr "file1.oc" "lambert3.c";
connectAttr "file1.ot" "lambert3.it";
connectAttr "lambert3.oc" "lambert3SG.ss";
connectAttr "particleShape1.iog" "lambert3SG.dsm" -na;
connectAttr "lambert3SG.msg" "materialInfo2.sg";
connectAttr "lambert3.msg" "materialInfo2.m";
connectAttr "file1.msg" "materialInfo2.t" -na;
connectAttr "place2dTexture1.c" "file1.c";
connectAttr "place2dTexture1.tf" "file1.tf";
connectAttr "place2dTexture1.rf" "file1.rf";
connectAttr "place2dTexture1.mu" "file1.mu";
connectAttr "place2dTexture1.mv" "file1.mv";
connectAttr "place2dTexture1.s" "file1.s";
connectAttr "place2dTexture1.wu" "file1.wu";
connectAttr "place2dTexture1.wv" "file1.wv";
connectAttr "place2dTexture1.re" "file1.re";
connectAttr "place2dTexture1.of" "file1.of";
connectAttr "place2dTexture1.r" "file1.ro";
connectAttr "place2dTexture1.n" "file1.n";
connectAttr "place2dTexture1.vt1" "file1.vt1";
connectAttr "place2dTexture1.vt2" "file1.vt2";
connectAttr "place2dTexture1.vt3" "file1.vt3";
connectAttr "place2dTexture1.vc1" "file1.vc1";
connectAttr "place2dTexture1.o" "file1.uv";
connectAttr "place2dTexture1.ofs" "file1.fs";
connectAttr "expression1.out[0]" "file1.fe";
connectAttr ":time1.o" "expression1.tim";
connectAttr "file1.sce" "expression1.in[0]";
connectAttr "file1.ece" "expression1.in[1]";
connectAttr "file1.msg" "expression1.obm";
connectAttr "spriteNumRamp1.msg" "spriteNumRampMapper1.cn";
connectAttr "spriteNumRamp1.oc" "spriteNumRampMapper1.cnc";
connectAttr "particleShape1.spriteNumRampU" "spriteNumRampMapper1.upp";
connectAttr "particleShape1.ageNormalized" "spriteNumRampMapper1.vpp";
connectAttr "lambert2SG.pa" ":renderPartition.st" -na;
connectAttr "lambert3SG.pa" ":renderPartition.st" -na;
connectAttr "mat_rocket.msg" ":defaultShaderList1.s" -na;
connectAttr "lambert3.msg" ":defaultShaderList1.s" -na;
connectAttr "place2dTexture1.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "lightLinker1.msg" ":lightList1.ln" -na;
connectAttr "file1.msg" ":defaultTextureList1.tx" -na;
connectAttr "spriteNumRamp1.msg" ":defaultTextureList1.tx" -na;
connectAttr "particleShape2.iog" ":initialParticleSE.dsm" -na;
// End of test_particles.ma
