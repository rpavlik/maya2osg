//Maya ASCII 2010 scene
//Name: pumpkin_png.ma
//Last modified: Wed, Nov 03, 2010 06:15:34 PM
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
	setAttr ".t" -type "double3" -2.5511862446766216 1.0743904267471693 0.20244151834590379 ;
	setAttr ".r" -type "double3" -21.338352730371255 1357.3999999997372 -1.2347295894003898e-014 ;
createNode camera -s -n "perspShape" -p "persp";
	setAttr -k off ".v" no;
	setAttr ".fl" 34.999999999999986;
	setAttr ".coi" 2.6290721049040515;
	setAttr ".imn" -type "string" "persp";
	setAttr ".den" -type "string" "persp_depth";
	setAttr ".man" -type "string" "persp_mask";
	setAttr ".tp" -type "double3" -1.1920928955078125e-007 0.23393440246582031 -1.7881393432617188e-007 ;
	setAttr ".hc" -type "string" "viewSet -p %camera";
createNode transform -s -n "top";
	setAttr ".v" no;
	setAttr ".t" -type "double3" -1.1920928955078125e-007 100.1 -1.788139121128597e-007 ;
	setAttr ".r" -type "double3" -89.999999999999986 0 0 ;
createNode camera -s -n "topShape" -p "top";
	setAttr -k off ".v" no;
	setAttr ".rnd" no;
	setAttr ".coi" 100.1;
	setAttr ".ow" 2.4684525669886912;
	setAttr ".imn" -type "string" "top";
	setAttr ".den" -type "string" "top_depth";
	setAttr ".man" -type "string" "top_mask";
	setAttr ".hc" -type "string" "viewSet -t %camera";
	setAttr ".o" yes;
createNode transform -s -n "front";
	setAttr ".v" no;
	setAttr ".t" -type "double3" -0.08556936959980338 0.19589727491405423 100.10001806338865 ;
createNode camera -s -n "frontShape" -p "front";
	setAttr -k off ".v" no;
	setAttr ".rnd" no;
	setAttr ".coi" 100.1;
	setAttr ".ow" 3.1469389503115535;
	setAttr ".imn" -type "string" "front";
	setAttr ".den" -type "string" "front_depth";
	setAttr ".man" -type "string" "front_mask";
	setAttr ".hc" -type "string" "viewSet -f %camera";
	setAttr ".o" yes;
createNode transform -s -n "side";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 100.10001806338866 0.060135602951049805 -1.7881391209950289e-007 ;
	setAttr ".r" -type "double3" 0 89.999999999999986 0 ;
createNode camera -s -n "sideShape" -p "side";
	setAttr -k off ".v" no;
	setAttr ".rnd" no;
	setAttr ".coi" 100.1;
	setAttr ".ow" 2.4198473149942461;
	setAttr ".imn" -type "string" "side";
	setAttr ".den" -type "string" "side_depth";
	setAttr ".man" -type "string" "side_mask";
	setAttr ".hc" -type "string" "viewSet -s %camera";
	setAttr ".o" yes;
createNode transform -n "pSphere1";
createNode mesh -n "pSphereShape1" -p "pSphere1";
	addAttr -ci true -sn "mso" -ln "miShadingSamplesOverride" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "msh" -ln "miShadingSamples" -min 0 -smx 8 -at "float";
	addAttr -ci true -sn "mdo" -ln "miMaxDisplaceOverride" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "mmd" -ln "miMaxDisplace" -min 0 -smx 1 -at "float";
	setAttr -k off ".v";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".pv" -type "double2" 0.44096630620446431 0.48741886274699853 ;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
createNode transform -n "pointLight1";
	setAttr ".t" -type "double3" -1.4767475095010076 1.4794298780919659 -0.26115812997459487 ;
createNode pointLight -n "pointLightShape1" -p "pointLight1";
	setAttr -k off ".v";
	setAttr ".us" no;
createNode lightLinker -n "lightLinker1";
	setAttr -s 4 ".lnk";
	setAttr -s 4 ".slnk";
createNode displayLayerManager -n "layerManager";
createNode displayLayer -n "defaultLayer";
createNode renderLayerManager -n "renderLayerManager";
createNode renderLayer -n "defaultRenderLayer";
	setAttr ".g" yes;
createNode polySphere -n "polySphere1";
createNode polyExtrudeFace -n "polyExtrudeFace1";
	setAttr ".ics" -type "componentList" 1 "f[380:399]";
	setAttr ".ix" -type "matrix" 1 0 0 0 0 1 0 0 0 0 1 0 0 0 0 1;
	setAttr ".ws" yes;
	setAttr ".pvt" -type "float3" -1.4901161e-008 0.69268793 -2.9802322e-008 ;
	setAttr ".rs" 52493;
	setAttr ".lt" -type "double3" -1.9983201291623454e-017 4.3368086899420177e-017 0.40376629589822438 ;
	setAttr ".c[0]"  0 1 1;
createNode polyTweak -n "polyTweak1";
	setAttr ".uopa" yes;
	setAttr -s 110 ".tk";
	setAttr ".tk[263:270]" -type "float3" 0 -0.051735058 0  0 -0.051735058 0  
		0 -0.051735058 0  0 -0.051735058 0  0 -0.051735058 0  0 -0.051735058 0  0 -0.051735058 
		0  0 -0.051735058 0 ;
	setAttr ".tk[280:379]" -type "float3" 0 -0.051735058 0  0 -0.051735058 0  
		0 -0.051735058 0  0 -0.051735058 0  0 -0.051735058 0  0 -0.051735058 0  0 -0.051735058 
		0  0 -0.051735058 0  0 -0.051735058 0  0 -0.051735058 0  0 -0.051735058 0  0 -0.051735058 
		0  0 -0.051735058 0  0 -0.051735058 0  0 -0.051735058 0  0 -0.051735058 0  0 -0.051735058 
		0  0 -0.051735058 0  0 -0.051735058 0  0 -0.051735058 0  0 -0.11377051 0  0 -0.11377051 
		0  0 -0.11377051 0  0 -0.11377051 0  0 -0.11377051 0  0 -0.11377051 0  0 -0.11377051 
		0  0 -0.11377051 0  0 -0.11377051 0  0 -0.11377051 0  0 -0.11377051 0  0 -0.11377051 
		0  0 -0.11377051 0  0 -0.11377051 0  0 -0.11377051 0  0 -0.11377051 0  0 -0.11377051 
		0  0 -0.11377051 0  0 -0.11377051 0  0 -0.11377051 0  0 -0.17037024 0  0 -0.17037024 
		0  0 -0.17037024 0  0 -0.17037024 0  0 -0.17037024 0  0 -0.17037024 0  0 -0.17037024 
		0  0 -0.17037024 0  0 -0.17037024 0  0 -0.17037024 0  0 -0.17037024 0  0 -0.17037024 
		0  0 -0.17037024 0  0 -0.17037024 0  0 -0.17037024 0  0 -0.17037024 0  0 -0.17037024 
		0  0 -0.17037024 0  0 -0.17037024 0  0 -0.17037024 0  0 -0.30115628 0  0 -0.30115628 
		0  0 -0.30115628 0  0 -0.30115628 0  0 -0.30115628 0  0 -0.30115628 0  0 -0.30115628 
		0  0 -0.30115628 0  0 -0.30115628 0  0 -0.30115628 0  0 -0.30115628 0  0 -0.30115628 
		0  0 -0.30115628 0  0 -0.30115628 0  0 -0.30115628 0  0 -0.30115628 0  -7.4505806e-009 
		-0.30115637 0  0 -0.30115628 0  0 -0.30115628 0  0 -0.30115628 0  -0.084645055 -0.297654 
		0.027502827  -0.072003379 -0.297654 0.052313499  -0.052313525 -0.297654 0.072003357  
		-0.027502839 -0.297654 0.084645011  -8.8792484e-009 -0.297654 0.089001045  0.027502831 
		-0.297654 0.084644996  0.052313499 -0.297654 0.07200335  0.07200335 -0.297654 0.052313469  
		0.084645003 -0.297654 0.027502816  0.089001037 -0.297654 -1.8561421e-008  0.084645003 
		-0.297654 -0.027502842  0.07200335 -0.297654 -0.052313522  0.052313477 -0.297654 
		-0.072003357  0.027502827 -0.297654 -0.084645011  -5.6246221e-009 -0.297654 -0.089001045  
		-0.027502833 -0.297654 -0.084645003  -0.052313492 -0.297654 -0.07200335  -0.07200335 
		-0.297654 -0.052313514  -0.084644996 -0.297654 -0.027502839  -0.089001022 -0.297654 
		-1.8561421e-008 ;
	setAttr ".tk[381]" -type "float3" -8.8792484e-009 -0.30465853 -1.8561421e-008 ;
createNode polyExtrudeFace -n "polyExtrudeFace2";
	setAttr ".ics" -type "componentList" 1 "f[380:399]";
	setAttr ".ix" -type "matrix" 1 0 0 0 0 1 0 0 0 0 1 0 0 0 0 1;
	setAttr ".ws" yes;
	setAttr ".pvt" -type "float3" -1.4901161e-008 0.87822014 1.3038516e-007 ;
	setAttr ".rs" 35578;
	setAttr ".lt" -type "double3" 8.4669413407539862e-018 1.7347234759768071e-017 0.15403517403465905 ;
	setAttr ".c[0]"  0 1 1;
createNode polyTweak -n "polyTweak2";
	setAttr ".uopa" yes;
	setAttr -s 21 ".tk[381:401]" -type "float3"  -0.02483763 -0.21672888 0.0080704093 
		-0.021128148 -0.21672881 0.015350516 -1.2376675e-009 -0.21846344 4.4314305e-008 -0.015350495 
		-0.21672888 0.021128017 -0.0080702342 -0.21672881 0.024837613 -1.0429797e-009 -0.21672881 
		0.026115829 0.0080702323 -0.21672881 0.024837758 0.015350497 -0.21672888 0.021128302 
		0.021128144 -0.2167289 0.01535066 0.02483763 -0.2167289 0.0080701271 0.026115822 
		-0.2167289 3.9755943e-008 0.024837628 -0.21672888 -0.0080700535 0.021128137 -0.21672881 
		-0.015350713 0.015350497 -0.21672879 -0.021128362 0.0080702277 -0.21672888 -0.024837743 
		-2.3927176e-009 -0.21672888 -0.026115829 -0.0080702323 -0.21672888 -0.024837399 -0.015350495 
		-0.21672888 -0.021127939 -0.021128144 -0.21672888 -0.015350303 -0.024837626 -0.21672888 
		-0.0080700498 -0.026115822 -0.21672888 3.9142417e-008;
createNode polyExtrudeFace -n "polyExtrudeFace3";
	setAttr ".ics" -type "componentList" 1 "f[380:399]";
	setAttr ".ix" -type "matrix" 1 0 0 0 0 1 0 0 0 0 1 0 0 0 0 1;
	setAttr ".ws" yes;
	setAttr ".pvt" -type "float3" 4.4703484e-008 0.83568925 -7.8231096e-008 ;
	setAttr ".rs" 56091;
	setAttr ".lt" -type "double3" 3.5744790374131474e-019 -2.4719809532669501e-017 0.08470576006855314 ;
	setAttr ".lr" -type "double3" 10.283137394464879 -25.640489711553911 4.1347848410382984 ;
	setAttr ".c[0]"  0 1 1;
createNode polyTweak -n "polyTweak3";
	setAttr ".uopa" yes;
	setAttr -s 42 ".tk";
	setAttr ".tk[381:421]" -type "float3" 0 -0.10108346 0  0 -0.10108346 0  0 
		-0.10108346 0  0 -0.10108346 0  0 -0.10108346 0  0 -0.10108346 0  0 -0.10108346 0  
		0 -0.10108346 0  0 -0.10108346 0  0 -0.10108346 0  0 -0.10108346 0  0 -0.10108346 
		0  0 -0.10108346 0  0 -0.10108346 0  0 -0.10108346 0  0 -0.10108346 0  0 -0.10108346 
		0  0 -0.10108346 0  0 -0.10108346 0  0 -0.10108346 0  0 -0.19639251 4.6566129e-010  
		1.8626451e-009 -0.19639251 0  0 -0.19639251 2.220446e-016  9.3132257e-010 -0.19639251 
		0  4.6566129e-010 -0.19639251 0  0 -0.19639251 -1.8626451e-009  -4.6566129e-010 -0.19639251 
		0  0 -0.19639251 -1.8626451e-009  -1.8626451e-009 -0.19639251 9.3132257e-010  -1.8626451e-009 
		-0.19639251 -4.6566129e-010  7.4505806e-009 -0.19639251 0  -1.8626451e-009 -0.19639251 
		-4.6566129e-010  0 -0.19639251 -9.3132257e-010  9.3132257e-010 -0.19639251 0  0 -0.19639251 
		0  0 -0.19639251 -1.8626451e-009  0 -0.19639251 -1.8626451e-009  -9.3132257e-010 
		-0.19639251 1.8626451e-009  0 -0.19639251 -9.3132257e-010  0 -0.19639251 -4.6566129e-010  
		-1.8626451e-009 -0.19639251 -1.0658141e-014 ;
createNode polyExtrudeFace -n "polyExtrudeFace4";
	setAttr ".ics" -type "componentList" 1 "f[380:399]";
	setAttr ".ix" -type "matrix" 1 0 0 0 0 1 0 0 0 0 1 0 0 0 0 1;
	setAttr ".ws" yes;
	setAttr ".pvt" -type "float3" 0.0010035411 0.91790164 -0.00037326664 ;
	setAttr ".rs" 38871;
	setAttr ".lt" -type "double3" 0.0014439898209584086 -0.014720999883455897 0.099902261434436632 ;
	setAttr ".lr" -type "double3" 8.4219050903494921 0.056847881475890968 5.218296635779053 ;
	setAttr ".c[0]"  0 1 1;
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
		+ "\t\t\t$editorName = $panelName;\n            outlinerEditor -e \n                -showShapes 1\n                -showAttributes 0\n                -showConnected 0\n                -showAnimCurvesOnly 0\n                -showMuteInfo 0\n                -organizeByLayer 1\n                -showAnimLayerWeight 1\n                -autoExpandLayers 1\n                -autoExpand 0\n                -showDagOnly 1\n                -showAssets 1\n                -showContainedOnly 1\n                -showPublishedAsConnected 0\n                -showContainerContents 1\n                -ignoreDagHierarchy 0\n                -expandConnections 0\n                -showUnitlessCurves 1\n                -showCompounds 1\n                -showLeafs 1\n                -showNumericAttrsOnly 0\n                -highlightActive 1\n                -autoSelectNewObjects 0\n                -doNotSelectNewObjects 0\n                -dropIsParent 1\n                -transmitFilters 0\n                -setFilter \"defaultSetFilter\" \n                -showSetMembers 1\n"
		+ "                -allowMultiSelection 1\n                -alwaysToggleSelect 0\n                -directSelect 0\n                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -containersIgnoreFilters 0\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n                -showUVAttrsOnly 0\n                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -animLayerFilterOptions \"allAffecting\" \n                -sortOrder \"none\" \n                -longNames 0\n                -niceNames 1\n                -showNamespace 1\n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\toutlinerPanel -edit -l (localizedPanelLabel(\"Outliner\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        outlinerEditor -e \n            -showShapes 1\n            -showAttributes 0\n            -showConnected 0\n            -showAnimCurvesOnly 0\n            -showMuteInfo 0\n"
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
		+ "\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\tif ($useSceneConfig) {\n        string $configName = `getPanel -cwl (localizedPanelLabel(\"Current Layout\"))`;\n        if (\"\" != $configName) {\n\t\t\tpanelConfiguration -edit -label (localizedPanelLabel(\"Current Layout\")) \n\t\t\t\t-defaultImage \"\"\n\t\t\t\t-image \"\"\n\t\t\t\t-sc false\n\t\t\t\t-configString \"global string $gMainPane; paneLayout -e -cn \\\"vertical2\\\" -ps 1 20 100 -ps 2 80 100 $gMainPane;\"\n\t\t\t\t-removeAllPanels\n\t\t\t\t-ap false\n\t\t\t\t\t(localizedPanelLabel(\"Outliner\")) \n\t\t\t\t\t\"outlinerPanel\"\n\t\t\t\t\t\"$panelName = `outlinerPanel -unParent -l (localizedPanelLabel(\\\"Outliner\\\")) -mbv $menusOkayInPanels `;\\n$editorName = $panelName;\\noutlinerEditor -e \\n    -showShapes 1\\n    -showAttributes 0\\n    -showConnected 0\\n    -showAnimCurvesOnly 0\\n    -showMuteInfo 0\\n    -organizeByLayer 1\\n    -showAnimLayerWeight 1\\n    -autoExpandLayers 1\\n    -autoExpand 0\\n    -showDagOnly 1\\n    -showAssets 1\\n    -showContainedOnly 1\\n    -showPublishedAsConnected 0\\n    -showContainerContents 1\\n    -ignoreDagHierarchy 0\\n    -expandConnections 0\\n    -showUnitlessCurves 1\\n    -showCompounds 1\\n    -showLeafs 1\\n    -showNumericAttrsOnly 0\\n    -highlightActive 1\\n    -autoSelectNewObjects 0\\n    -doNotSelectNewObjects 0\\n    -dropIsParent 1\\n    -transmitFilters 0\\n    -setFilter \\\"defaultSetFilter\\\" \\n    -showSetMembers 1\\n    -allowMultiSelection 1\\n    -alwaysToggleSelect 0\\n    -directSelect 0\\n    -displayMode \\\"DAG\\\" \\n    -expandObjects 0\\n    -setsIgnoreFilters 1\\n    -containersIgnoreFilters 0\\n    -editAttrName 0\\n    -showAttrValues 0\\n    -highlightSecondary 0\\n    -showUVAttrsOnly 0\\n    -showTextureNodesOnly 0\\n    -attrAlphaOrder \\\"default\\\" \\n    -animLayerFilterOptions \\\"allAffecting\\\" \\n    -sortOrder \\\"none\\\" \\n    -longNames 0\\n    -niceNames 1\\n    -showNamespace 1\\n    $editorName\"\n"
		+ "\t\t\t\t\t\"outlinerPanel -edit -l (localizedPanelLabel(\\\"Outliner\\\")) -mbv $menusOkayInPanels  $panelName;\\n$editorName = $panelName;\\noutlinerEditor -e \\n    -showShapes 1\\n    -showAttributes 0\\n    -showConnected 0\\n    -showAnimCurvesOnly 0\\n    -showMuteInfo 0\\n    -organizeByLayer 1\\n    -showAnimLayerWeight 1\\n    -autoExpandLayers 1\\n    -autoExpand 0\\n    -showDagOnly 1\\n    -showAssets 1\\n    -showContainedOnly 1\\n    -showPublishedAsConnected 0\\n    -showContainerContents 1\\n    -ignoreDagHierarchy 0\\n    -expandConnections 0\\n    -showUnitlessCurves 1\\n    -showCompounds 1\\n    -showLeafs 1\\n    -showNumericAttrsOnly 0\\n    -highlightActive 1\\n    -autoSelectNewObjects 0\\n    -doNotSelectNewObjects 0\\n    -dropIsParent 1\\n    -transmitFilters 0\\n    -setFilter \\\"defaultSetFilter\\\" \\n    -showSetMembers 1\\n    -allowMultiSelection 1\\n    -alwaysToggleSelect 0\\n    -directSelect 0\\n    -displayMode \\\"DAG\\\" \\n    -expandObjects 0\\n    -setsIgnoreFilters 1\\n    -containersIgnoreFilters 0\\n    -editAttrName 0\\n    -showAttrValues 0\\n    -highlightSecondary 0\\n    -showUVAttrsOnly 0\\n    -showTextureNodesOnly 0\\n    -attrAlphaOrder \\\"default\\\" \\n    -animLayerFilterOptions \\\"allAffecting\\\" \\n    -sortOrder \\\"none\\\" \\n    -longNames 0\\n    -niceNames 1\\n    -showNamespace 1\\n    $editorName\"\n"
		+ "\t\t\t\t-ap false\n\t\t\t\t\t(localizedPanelLabel(\"Persp View\")) \n\t\t\t\t\t\"modelPanel\"\n"
		+ "\t\t\t\t\t\"$panelName = `modelPanel -unParent -l (localizedPanelLabel(\\\"Persp View\\\")) -mbv $menusOkayInPanels `;\\n$editorName = $panelName;\\nmodelEditor -e \\n    -cam `findStartUpCamera persp` \\n    -useInteractiveMode 0\\n    -displayLights \\\"default\\\" \\n    -displayAppearance \\\"smoothShaded\\\" \\n    -activeOnly 0\\n    -wireframeOnShaded 0\\n    -headsUpDisplay 1\\n    -selectionHiliteDisplay 1\\n    -useDefaultMaterial 0\\n    -bufferMode \\\"double\\\" \\n    -twoSidedLighting 1\\n    -backfaceCulling 0\\n    -xray 0\\n    -jointXray 0\\n    -activeComponentsXray 0\\n    -displayTextures 1\\n    -smoothWireframe 0\\n    -lineWidth 1\\n    -textureAnisotropic 0\\n    -textureHilight 1\\n    -textureSampling 2\\n    -textureDisplay \\\"modulate\\\" \\n    -textureMaxSize 8192\\n    -fogging 0\\n    -fogSource \\\"fragment\\\" \\n    -fogMode \\\"linear\\\" \\n    -fogStart 0\\n    -fogEnd 100\\n    -fogDensity 0.1\\n    -fogColor 0.5 0.5 0.5 1 \\n    -maxConstantTransparency 1\\n    -rendererName \\\"base_OpenGL_Renderer\\\" \\n    -colorResolution 256 256 \\n    -bumpResolution 512 512 \\n    -textureCompression 0\\n    -transparencyAlgorithm \\\"frontAndBackCull\\\" \\n    -transpInShadows 0\\n    -cullingOverride \\\"none\\\" \\n    -lowQualityLighting 0\\n    -maximumNumHardwareLights 1\\n    -occlusionCulling 0\\n    -shadingModel 0\\n    -useBaseRenderer 0\\n    -useReducedRenderer 0\\n    -smallObjectCulling 0\\n    -smallObjectThreshold -1 \\n    -interactiveDisableShadows 0\\n    -interactiveBackFaceCull 0\\n    -sortTransparent 1\\n    -nurbsCurves 1\\n    -nurbsSurfaces 1\\n    -polymeshes 1\\n    -subdivSurfaces 1\\n    -planes 1\\n    -lights 1\\n    -cameras 1\\n    -controlVertices 1\\n    -hulls 1\\n    -grid 1\\n    -joints 1\\n    -ikHandles 1\\n    -deformers 1\\n    -dynamics 1\\n    -fluids 1\\n    -hairSystems 1\\n    -follicles 1\\n    -nCloths 1\\n    -nParticles 1\\n    -nRigids 1\\n    -dynamicConstraints 1\\n    -locators 1\\n    -manipulators 1\\n    -dimensions 1\\n    -handles 1\\n    -pivots 1\\n    -textures 1\\n    -strokes 1\\n    -shadows 0\\n    $editorName;\\nmodelEditor -e -viewSelected 0 $editorName\"\n"
		+ "\t\t\t\t\t\"modelPanel -edit -l (localizedPanelLabel(\\\"Persp View\\\")) -mbv $menusOkayInPanels  $panelName;\\n$editorName = $panelName;\\nmodelEditor -e \\n    -cam `findStartUpCamera persp` \\n    -useInteractiveMode 0\\n    -displayLights \\\"default\\\" \\n    -displayAppearance \\\"smoothShaded\\\" \\n    -activeOnly 0\\n    -wireframeOnShaded 0\\n    -headsUpDisplay 1\\n    -selectionHiliteDisplay 1\\n    -useDefaultMaterial 0\\n    -bufferMode \\\"double\\\" \\n    -twoSidedLighting 1\\n    -backfaceCulling 0\\n    -xray 0\\n    -jointXray 0\\n    -activeComponentsXray 0\\n    -displayTextures 1\\n    -smoothWireframe 0\\n    -lineWidth 1\\n    -textureAnisotropic 0\\n    -textureHilight 1\\n    -textureSampling 2\\n    -textureDisplay \\\"modulate\\\" \\n    -textureMaxSize 8192\\n    -fogging 0\\n    -fogSource \\\"fragment\\\" \\n    -fogMode \\\"linear\\\" \\n    -fogStart 0\\n    -fogEnd 100\\n    -fogDensity 0.1\\n    -fogColor 0.5 0.5 0.5 1 \\n    -maxConstantTransparency 1\\n    -rendererName \\\"base_OpenGL_Renderer\\\" \\n    -colorResolution 256 256 \\n    -bumpResolution 512 512 \\n    -textureCompression 0\\n    -transparencyAlgorithm \\\"frontAndBackCull\\\" \\n    -transpInShadows 0\\n    -cullingOverride \\\"none\\\" \\n    -lowQualityLighting 0\\n    -maximumNumHardwareLights 1\\n    -occlusionCulling 0\\n    -shadingModel 0\\n    -useBaseRenderer 0\\n    -useReducedRenderer 0\\n    -smallObjectCulling 0\\n    -smallObjectThreshold -1 \\n    -interactiveDisableShadows 0\\n    -interactiveBackFaceCull 0\\n    -sortTransparent 1\\n    -nurbsCurves 1\\n    -nurbsSurfaces 1\\n    -polymeshes 1\\n    -subdivSurfaces 1\\n    -planes 1\\n    -lights 1\\n    -cameras 1\\n    -controlVertices 1\\n    -hulls 1\\n    -grid 1\\n    -joints 1\\n    -ikHandles 1\\n    -deformers 1\\n    -dynamics 1\\n    -fluids 1\\n    -hairSystems 1\\n    -follicles 1\\n    -nCloths 1\\n    -nParticles 1\\n    -nRigids 1\\n    -dynamicConstraints 1\\n    -locators 1\\n    -manipulators 1\\n    -dimensions 1\\n    -handles 1\\n    -pivots 1\\n    -textures 1\\n    -strokes 1\\n    -shadows 0\\n    $editorName;\\nmodelEditor -e -viewSelected 0 $editorName\"\n"
		+ "\t\t\t\t$configName;\n\n            setNamedPanelLayout (localizedPanelLabel(\"Current Layout\"));\n        }\n\n        panelHistory -e -clear mainPanelHistory;\n        setFocus `paneLayout -q -p1 $gMainPane`;\n        sceneUIReplacement -deleteRemaining;\n        sceneUIReplacement -clear;\n\t}\n\n\ngrid -spacing 5 -size 12 -divisions 5 -displayAxes yes -displayGridLines yes -displayDivisionLines yes -displayPerspectiveLabels no -displayOrthographicLabels no -displayAxesBold yes -perspectiveLabelPosition axis -orthographicLabelPosition edge;\nviewManip -drawCompass 0 -compassAngle 0 -frontParameters \"\" -homeParameters \"\" -selectionLockParameters \"\";\n}\n");
	setAttr ".st" 3;
createNode script -n "sceneConfigurationScriptNode";
	setAttr ".b" -type "string" "playbackOptions -min 1 -max 24 -ast 1 -aet 48 ";
	setAttr ".st" 6;
createNode lambert -n "lambert_pumpkin";
createNode shadingEngine -n "lambert2SG";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode materialInfo -n "materialInfo1";
createNode polyAutoProj -n "polyAutoProj1";
	setAttr ".uopa" yes;
	setAttr ".ics" -type "componentList" 1 "f[0:479]";
	setAttr ".ix" -type "matrix" 1 0 0 0 0 1 0 0 0 0 1 0 0 0 0 1;
	setAttr ".ws" yes;
	setAttr ".t" -type "double3" -1.1920928955078125e-007 0.31615477800369263 -1.7881393432617188e-007 ;
	setAttr ".s" -type "double3" 2.0000005960464478 2.0000005960464478 2.0000005960464478 ;
	setAttr ".pvt" -type "float3" -1.1920929e-007 0.31615478 -1.7881393e-007 ;
	setAttr ".ps" 0.20000000298023224;
	setAttr ".dl" yes;
createNode polyTweak -n "polyTweak4";
	setAttr ".uopa" yes;
	setAttr -s 362 ".tk";
	setAttr ".tk[0:159]" -type "float3" 0 0.64877677 0  0 0.64877677 0  0 0.64877677 
		0  0 0.64877677 0  0 0.64877677 0  0 0.64877677 0  0 0.64877677 0  0 0.64877677 0  
		0 0.64877677 0  0 0.64877677 0  0 0.64877677 0  0 0.64877677 0  0 0.64877677 0  0 
		0.64877677 0  0 0.64877677 0  0 0.64877677 0  0 0.64877677 0  0 0.64877677 0  0 0.64877677 
		0  0 0.64877677 0  0 0.57202578 0  0 0.57202578 0  0 0.57202578 0  0 0.57202578 0  
		0 0.57202578 0  0 0.57202578 0  0 0.57202578 0  0 0.57202578 0  0 0.57202578 0  0 
		0.57202578 0  0 0.57202578 0  0 0.57202578 0  0 0.57202578 0  0 0.57202578 0  0 0.57202578 
		0  0 0.57202578 0  0 0.57202578 0  0 0.57202578 0  0 0.57202578 0  0 0.57202578 0  
		0 0.48719603 0  0 0.48719603 0  0 0.48719603 0  0 0.48719603 0  0 0.48719603 0  0 
		0.48719603 0  0 0.48719603 0  0 0.48719603 0  0 0.48719603 0  0 0.48719603 0  0 0.48719603 
		0  0 0.48719603 0  0 0.48719603 0  0 0.48719603 0  0 0.48719603 0  0 0.48719603 0  
		0 0.48719603 0  0 0.48719603 0  0 0.48719603 0  0 0.48719603 0  0 0.41271406 0  0 
		0.41271406 0  0 0.41271406 0  0 0.41271406 0  0 0.41271406 0  0 0.41271406 0  0 0.41271406 
		0  0 0.41271406 0  0 0.41271406 0  0 0.41271406 0  0 0.41271406 0  0 0.41271406 0  
		0 0.41271406 0  0 0.41271406 0  0 0.41271406 0  0 0.41271406 0  0 0.41271406 0  0 
		0.41271406 0  0 0.41271406 0  0 0.41271406 0  0 0.33823204 0  0 0.33823204 0  0 0.33823204 
		0  0 0.33823204 0  0 0.33823204 0  0 0.33823204 0  0 0.33823204 0  0 0.33823204 0  
		0 0.33823204 0  0 0.33823204 0  0 0.33823204 0  0 0.33823204 0  0 0.33823204 0  0 
		0.33823204 0  0 0.33823204 0  0 0.33823204 0  0 0.33823204 0  0 0.33823204 0  0 0.33823204 
		0  0 0.33823204 0  0 0.2554743 0  0 0.2554743 0  0 0.2554743 0  0 0.2554743 0  0 
		0.2554743 0  0 0.2554743 0  0 0.2554743 0  0 0.2554743 0  0 0.2554743 0  0 0.2554743 
		0  0 0.2554743 0  0 0.2554743 0  0 0.2554743 0  0 0.2554743 0  0 0.2554743 0  0 0.2554743 
		0  0 0.2554743 0  0 0.2554743 0  0 0.2554743 0  0 0.2554743 0  0 0.16444074 0  0 
		0.16444074 0  0 0.16444074 0  0 0.16444074 0  0 0.16444074 0  0 0.16444074 0  0 0.16444074 
		0  0 0.16444074 0  0 0.16444074 0  0 0.16444074 0  0 0.16444074 0  0 0.16444074 0  
		0 0.16444074 0  0 0.16444074 0  0 0.16444074 0  0 0.16444074 0  0 0.16444074 0  0 
		0.16444074 0  0 0.16444074 0  0 0.16444074 0  0 0.070629552 0  0 0.070629552 0  0 
		0.070629552 0  0 0.070629552 0  0 0.070629552 0  0 0.070629552 0  0 0.070629552 0  
		0 0.070629552 0  0 0.070629552 0  0 0.070629552 0  0 0.070629552 0  0 0.070629552 
		0  0 0.070629552 0  0 0.070629552 0  0 0.070629552 0  0 0.070629552 0  0 0.070629552 
		0  0 0.070629552 0  0 0.070629552 0  0 0.070629552 0 ;
	setAttr ".tk[260:461]" -type "float3" 0 -5.2154064e-008 0  0 -5.2154064e-008 
		0  0 -5.2154064e-008 0  0 0.043760687 0  0 0.043760687 0  0 0.043760687 0  0 0.043760687 
		0  0 0.043760687 0  0 0.043760687 0  0 0.043760687 0  0 0.043760687 0  0 -5.2154064e-008 
		0  0 -5.2154064e-008 0  0 -5.2154064e-008 0  0 -5.2154064e-008 0  0 -5.2154064e-008 
		0  0 -5.2154064e-008 0  0 -5.2154064e-008 0  0 -5.2154064e-008 0  0 -5.2154064e-008 
		0  0 -5.2154064e-008 0  0 -5.2154064e-008 0  0 -5.2154064e-008 0  0 -5.2154064e-008 
		0  0 -5.2154064e-008 0  0 -5.2154064e-008 0  0 -5.2154064e-008 0  0 -5.2154064e-008 
		0  0 -5.2154064e-008 0  0 -5.2154064e-008 0  0 -5.2154064e-008 0  0 -5.2154064e-008 
		0  0 -5.2154064e-008 0  0 -5.2154064e-008 0  0 -5.2154064e-008 0  0 -5.2154064e-008 
		0  0 -5.2154064e-008 0  0 -5.2154064e-008 0  0 -5.2154064e-008 0  0 -5.2154064e-008 
		0  0 -5.2154064e-008 0  0 -5.2154064e-008 0  0 -5.2154064e-008 0  0 -5.2154064e-008 
		0  0 -5.2154064e-008 0  0 -5.2154064e-008 0  0 -5.2154064e-008 0  0 -5.2154064e-008 
		0  0 -5.2154064e-008 0  0 -5.2154064e-008 0  0 -5.2154064e-008 0  0 -5.2154064e-008 
		0  0 -5.2154064e-008 0  0 -5.2154064e-008 0  0 -5.2154064e-008 0  0 -5.2154064e-008 
		0  0 -5.2154064e-008 0  0 -5.2154064e-008 0  0 -5.2154064e-008 0  0 -5.2154064e-008 
		0  0 -5.2154064e-008 0  0 -5.2154064e-008 0  0 -5.2154064e-008 0  0 -5.2154064e-008 
		0  0 -5.2154064e-008 0  0 -5.2154064e-008 0  0 -5.2154064e-008 0  0 -5.2154064e-008 
		0  0 -5.2154064e-008 0  0 -5.2154064e-008 0  0 -5.2154064e-008 0  0 -5.2154064e-008 
		0  0 -5.2154064e-008 0  0 -5.2154064e-008 0  0 -5.2154064e-008 0  0 -5.2154064e-008 
		0  0 -5.2154064e-008 0  0 -5.2154064e-008 0  0 -5.2154064e-008 0  0 -5.2154064e-008 
		0  0 -5.2154064e-008 0  0 -5.2154064e-008 0  0 -5.2154064e-008 0  0 -5.2154064e-008 
		0  0 -5.2154064e-008 0  0 -5.2154064e-008 0  0 -5.2154064e-008 0  0 -5.2154064e-008 
		0  0 -5.2154064e-008 0  0 -5.2154064e-008 0  0 -5.2154064e-008 0  0 -5.2154064e-008 
		0  0 -5.2154064e-008 0  0 -5.2154064e-008 0  0 -5.2154064e-008 0  0 -5.2154064e-008 
		0  0 -5.2154064e-008 0  0 -5.2154064e-008 0  0 -5.2154064e-008 0  0 -5.2154064e-008 
		0  0 -5.2154064e-008 0  0 -5.2154064e-008 0  0 -5.2154064e-008 0  0 -5.2154064e-008 
		0  0 -5.2154064e-008 0  0 -5.2154064e-008 0  0 -5.2154064e-008 0  0 -5.2154064e-008 
		0  0 -5.2154064e-008 0  0 -5.2154064e-008 0  0 -5.2154064e-008 0  0 -5.2154064e-008 
		0  0 -5.2154064e-008 0  0 -5.2154064e-008 0  0 -5.2154064e-008 0  0 -5.2154064e-008 
		0  0 -5.2154064e-008 0  0 -5.2154064e-008 0  0 -5.2154064e-008 0  0 -5.2154064e-008 
		0  0 0.64877677 0  0 -5.2154064e-008 0  0 -5.2154064e-008 0  0 -5.2154064e-008 0  
		0 -5.2154064e-008 0  0 -5.2154064e-008 0  0 -5.2154064e-008 0  0 -5.2154064e-008 
		0  0 -5.2154064e-008 0  0 -5.2154064e-008 0  0 -5.2154064e-008 0  0 -5.2154064e-008 
		0  0 -5.2154064e-008 0  0 -5.2154064e-008 0  0 -5.2154064e-008 0  0 -5.2154064e-008 
		0  0 -5.2154064e-008 0  0 -5.2154064e-008 0  0 -5.2154064e-008 0  0 -5.2154064e-008 
		0  0 -5.2154064e-008 0  0 -5.2154064e-008 0  0 -5.2154064e-008 0  0 -5.2154064e-008 
		0  0 -5.2154064e-008 0  0 -5.2154064e-008 0  0 -5.2154064e-008 0  0 -5.2154064e-008 
		0  0 -5.2154064e-008 0  0 -5.2154064e-008 0  0 -5.2154064e-008 0  0 -5.2154064e-008 
		0  0 -5.2154064e-008 0  0 -5.2154064e-008 0  0 -5.2154064e-008 0  0 -5.2154064e-008 
		0  0 -5.2154064e-008 0  0 -5.2154064e-008 0  0 -5.2154064e-008 0  0 -5.2154064e-008 
		0  0 -5.2154064e-008 0  0 -5.2154064e-008 0  0 -5.2154064e-008 0  0 -5.2154064e-008 
		0  0 -5.2154064e-008 0  0 -5.2154064e-008 0  0 -5.2154064e-008 0  0 -5.2154064e-008 
		0  0 -5.2154064e-008 0  0 -5.2154064e-008 0  0 -5.2154064e-008 0  0 -5.2154064e-008 
		0  0 -5.2154064e-008 0  0 -5.2154064e-008 0  0 -5.2154064e-008 0  0 -5.2154064e-008 
		0  0 -5.2154064e-008 0  0 -5.2154064e-008 0  0 -5.2154064e-008 0  0 -5.2154064e-008 
		0  0 -5.2154064e-008 0  0 -5.2154064e-008 0  0 -5.2154064e-008 0  0 -5.2154064e-008 
		0  0 -5.2154064e-008 0  0 -5.2154064e-008 0  0 -5.2154064e-008 0  0 -5.2154064e-008 
		0  0 -5.2154064e-008 0  0 -5.2154064e-008 0  0 -5.2154064e-008 0  0 -5.2154064e-008 
		0  0 -5.2154064e-008 0  0 -5.2154064e-008 0  0 -5.2154064e-008 0  0 -5.2154064e-008 
		0  0 -5.2154064e-008 0  0 -5.2154064e-008 0  0 -5.2154064e-008 0  0 -5.2154064e-008 
		0  0 -5.2154064e-008 0  0 -5.2154064e-008 0 ;
createNode polySmoothFace -n "polySmoothFace1";
	setAttr ".ics" -type "componentList" 1 "f[*]";
	setAttr ".dv" 0;
	setAttr ".suv" yes;
	setAttr ".ps" 0.10000000149011612;
	setAttr ".ro" 1;
	setAttr ".ma" yes;
	setAttr ".m08" yes;
createNode polyTweakUV -n "polyTweakUV1";
	setAttr ".uopa" yes;
	setAttr -s 599 ".uvtk";
	setAttr ".uvtk[0:249]" -type "float2" 0.013876059 0.0092506995 0.013876044 
		0.0092506995 0.013876059 0.0092506995 0.013876059 0.0092506995 0.013876044 0.0092506995 
		0.013876044 0.0092506995 0.013876059 0.0092506995 0.013876059 0.0092506995 0.013876059 
		0.0092506995 0.013876044 0.0092506995 0.013876059 0.0092506995 0.013876044 0.0092506995 
		0.013876044 0.0092506995 0.013876059 0.0092506995 0.013876059 0.0092506995 0.013876059 
		0.0092506995 0.013876059 0.0092506995 0.013876059 0.0092506995 0.013876059 0.0092506995 
		0.013876044 0.0092506995 0.013876044 0.0092506995 0.013876059 0.0092506921 0.013876059 
		0.0092506995 0.013876059 0.0092506921 0.013876059 0.0092506995 0.013876059 0.0092506995 
		0.013876059 0.0092506846 0.013876044 0.0092506995 0.013876059 0.0092506995 0.013876044 
		0.0092506995 0.013876044 0.0092506995 0.013876044 0.0092506995 0.013876059 0.0092506995 
		0.013876059 0.0092506995 0.013876059 0.0092506995 0.013876059 0.0092506995 0.013876059 
		0.0092506995 0.013876059 0.0092506995 0.013876059 0.0092506995 0.013876044 0.0092506995 
		0.013876059 0.0092506995 0.013876044 0.0092506995 0.013876044 0.0092506995 0.013876044 
		0.0092506921 0.013876044 0.0092506995 0.013876059 0.0092506995 0.013876059 0.0092506995 
		0.013876059 0.0092506995 0.013876059 0.0092506995 0.013876059 0.0092506995 0.013876059 
		0.0092506995 0.013876059 0.0092506995 0.013876059 0.0092506995 0.013876044 0.0092506846 
		0.013876044 0.0092506995 0.013876044 0.0092506995 0.013876044 0.0092506995 0.013876044 
		0.0092506995 0.013876044 0.0092506995 0.013876044 0.0092506995 0.013876059 0.0092506958 
		0.013876059 0.0092506958 0.013876059 0.0092506995 0.013876059 0.0092506995 0.013876059 
		0.0092506995 0.013876059 0.0092506995 0.013876059 0.0092506995 0.013876059 0.0092506995 
		0.013876059 0.0092506995 0.013876044 0.0092506846 0.013876044 0.0092506995 0.013876044 
		0.0092506995 0.013876044 0.0092506995 0.013876044 0.0092506995 0.013876044 0.0092506995 
		0.013876044 0.0092506995 0.013876044 0.0092506995 0.013876059 0.0092506958 0.013876059 
		0.0092506995 0.013876059 0.0092506995 0.013876059 0.0092506995 0.013876059 0.0092506995 
		0.013876059 0.0092506995 0.013876059 0.0092506995 0.013876044 0.0092506995 0.013876044 
		0.0092506846 0.013876044 0.0092506995 0.013876044 0.0092506995 0.013876044 0.0092506995 
		0.013876044 0.0092506995 0.013876044 0.0092506995 0.013876044 0.0092506995 0.013876044 
		0.0092506958 0.013876044 0.0092506977 0.013876059 0.0092506977 0.013876059 0.0092506995 
		0.013876059 0.0092506995 0.013876059 0.0092506995 0.013876059 0.0092506995 0.013876044 
		0.0092506995 0.013876044 0.0092506995 0.013876044 0.0092506995 0.013876044 0.0092506995 
		0.013876044 0.0092506995 0.013876044 0.0092506995 0.013876052 0.0092506995 0.013876044 
		0.0092506995 0.013876044 0.0092506995 0.013876044 0.0092506995 0.013876044 0.0092506958 
		0.013876044 0.0092506977 0.013876059 0.0092506995 0.013876059 0.0092506995 0.013876059 
		0.0092506995 0.013876044 0.0092506995 0.013876044 0.0092506995 0.013876044 0.0092506995 
		0.013876044 0.0092506995 0.013876044 0.0092506995 0.013876044 0.0092506846 0.013876044 
		0.0092506995 0.013876044 0.0092506995 0.013876044 0.0092506995 0.013876044 0.0092506958 
		0.013876044 0.0092506977 0.013876059 0.0092506995 0.013876059 0.0092506995 0.013876044 
		0.0092506995 0.013876044 0.0092506995 0.013876044 0.0092506995 0.013876044 0.0092506995 
		0.013876044 0.0092506995 0.013876044 0.0092506995 0.013876048 0.0092506995 0.013876048 
		0.0092506995 0.013876044 0.0092506958 0.013876059 0.0092506995 0.013876044 0.0092506995 
		0.013876044 0.0092506995 0.013876044 0.0092506995 0.013876044 0.0092506995 0.013876044 
		0.0092506995 0.013876044 0.0092506995 0.013876048 0.0092506995 0.013876048 0.0092506995 
		0.013876044 0.0092506995 0.013876044 0.0092506995 0.013876044 0.0092506995 0.013876044 
		0.0092506995 0.013876048 0.0092506995 0.013876047 0.0092506995 0.013876045 0.0092506995 
		0.013876044 0.0092506995 0.013876044 0.0092506995 0.013876044 0.0092506995 0.013876048 
		0.0092506995 0.013876047 0.0092506995 0.013876044 0.0092506995 0.013876048 0.0092506995 
		0.013876045 0.0092506995 0.013876048 0.0092506995 0.020814041 0.0277521 0.020814041 
		0.027752092 0.02081407 0.027752092 0.02081407 0.0277521 0.020814041 0.02775209 0.02081407 
		0.027752092 0.02081407 0.027752092 0.02081407 0.0277521 0.020814041 0.0277521 0.02081407 
		0.0277521 0.02081407 0.027752094 0.02081407 0.027752092 0.02081407 0.0277521 0.02081407 
		0.0277521 0.020814041 0.027752085 0.02081407 0.02775207 0.02081407 0.027752092 0.02081407 
		0.027752092 0.02081407 0.0277521 0.02081407 0.0277521 0.02081407 0.02775207 0.020814041 
		0.027752085 0.02081407 0.027752085 0.02081407 0.02775209 0.02081407 0.027752092 0.02081407 
		0.0277521 0.02081407 0.0277521 0.02081407 0.02775207 0.02081407 0.027752085 0.020814041 
		0.027752085 0.02081407 0.027752085 0.02081407 0.027752096 0.02081407 0.027752092 
		0.02081407 0.0277521 0.02081407 0.0277521 0.02081407 0.027752085 0.02081407 0.027752085 
		0.02081407 0.027752085 0.02081407 0.027752092 0.02081407 0.0277521 0.02081407 0.027752085 
		0.02081407 0.027752085 0.02081407 0.027752085 0.02081407 0.0277521 0.02081407 0.0277521 
		0.02081407 0.027752085 0.02081407 0.027752085 0.02081407 0.027752085 0.02081407 0.027752085 
		0.02081407 0.027752085 0.02081407 0.027752085 0.02081407 0.027752085 0.3885293 -0.034690104 
		0.3885293 -0.034690104 0.38852936 -0.034690104 0.38852936 -0.034690104 0.3885293 
		-0.034690104 0.38852936 -0.034690104 0.38852936 -0.034690104 0.38852936 -0.034690104 
		0.3885293 -0.034690104 0.38852936 -0.034690104 0.38852936 -0.034690104 0.3885293 
		-0.034690104 0.3885293 -0.034690104 0.3885293 -0.034690104 0.38852936 -0.034690104 
		0.38852936 -0.034690104 0.3885293 -0.034690104 0.3885293 -0.034690104 0.3885293 -0.034690104 
		0.3885293 -0.034690104 0.38852936 -0.034690104 0.38852936 -0.034690104 0.3885293 
		-0.034690104 0.3885293 -0.034690104 0.38852924 -0.034690104 0.38852924 -0.034690104 
		0.38852936 -0.034690104 0.3885293 -0.034690104 0.3885293 -0.034690104 0.38852924 
		-0.034690104 0.38852924 -0.034690104 0.3885293 -0.034690104 0.3885293 -0.034690104 
		0.3885293 -0.034690104 0.38852924 -0.034690104 0.38852924 -0.034690104 0.3885293 
		-0.034690104;
	setAttr ".uvtk[250:499]" 0.3885293 -0.034690104 0.3885293 -0.034690104 0.38852924 
		-0.034690104 0.38852924 -0.034690104 0.3885293 -0.034690104 0.38852924 -0.034690104 
		0.38852924 -0.034690104 0.3885293 -0.034690104 0.38852924 -0.034690104 0.3885293 
		-0.034690104 0.38852936 -0.034690104 0.38852936 -0.034690104 0.3885293 -0.034690104 
		0.3885293 -0.034690104 0.38852936 -0.034690104 0.3885293 -0.034690104 0.3885293 -0.034690104 
		0.3885293 -0.034690104 0.38852936 -0.034690134 0.3885293 -0.034690134 0.3885293 -0.034690104 
		0.3885293 -0.034690104 0.3885293 -0.034690104 0.38852936 -0.034690104 0.3885293 -0.034690104 
		0.3885293 -0.034690104 0.3885293 -0.034690104 0.3885293 -0.034690104 0.3885293 -0.034690104 
		0.38852936 -0.034690104 0.3885293 -0.034690104 0.3885293 -0.034690104 0.3885293 -0.034690134 
		0.3885293 -0.034690104 0.3885293 -0.034690104 0.3885293 -0.034690104 0.3885293 -0.034690104 
		0.3885293 -0.034690104 0.3885293 -0.034690134 0.3885293 -0.034690104 0.3885293 -0.034690104 
		0.3885293 -0.034690104 0.3885293 -0.034690104 0.3885293 -0.034690104 0.3885293 -0.034690104 
		0.3885293 -0.034690104 0.3885293 -0.034690104 0.3885293 -0.034690104 0.3885293 -0.034690104 
		0.3885293 -0.034690104 0.3885293 -0.034690104 0.3885293 -0.034690104 0.3885293 -0.034690104 
		0.3885293 -0.034690104 0.3885293 -0.034690104 0.3885293 -0.034690104 0.3885293 -0.034690104 
		0.039315466 -0.30064768 0.039315466 -0.30064768 0.039315462 -0.30064768 0.039315462 
		-0.30064768 0.039315466 -0.30064768 0.039315462 -0.30064768 0.039315462 -0.30064768 
		0.039315462 -0.30064768 0.039315466 -0.30064768 0.039315462 -0.30064771 0.039315462 
		-0.30064768 0.039315466 -0.30064768 0.039315466 -0.30064768 0.039315466 -0.30064768 
		0.039315462 -0.30064768 0.039315462 -0.30064765 0.039315466 -0.30064768 0.039315466 
		-0.30064768 0.039315466 -0.30064768 0.039315462 -0.30064768 0.039315466 -0.30064771 
		0.039315466 -0.30064768 0.039315466 -0.30064768 0.039315466 -0.30064768 0.039315466 
		-0.30064768 0.039315466 -0.30064768 0.039315466 -0.30064768 0.039315466 -0.30064768 
		0.039315466 -0.30064768 0.039315466 -0.30064768 0.039315466 -0.30064765 0.039315466 
		-0.30064768 0.039315466 -0.30064768 0.039315466 -0.30064768 0.039315466 -0.30064768 
		0.039315466 -0.30064768 0.039315466 -0.30064768 0.039315466 -0.30064771 0.060129572 
		-0.34921384 0.060129572 -0.34921384 0.060129572 -0.34921384 0.060129572 -0.34921384 
		0.060129572 -0.34921384 0.060129572 -0.34921384 0.060129572 -0.34921384 0.060129572 
		-0.34921384 0.060129572 -0.34921384 0.060129572 -0.34921384 0.060129572 -0.34921384 
		0.060129572 -0.34921384 0.060129572 -0.34921384 0.060129572 -0.34921384 0.060129572 
		-0.34921384 0.060129572 -0.34921384 0.060129572 -0.34921384 0.060129572 -0.34921384 
		0.060129572 -0.34921384 0.060129572 -0.34921384 0.060129572 -0.34921384 0.060129572 
		-0.34921384 0.060129572 -0.34921384 0.060129572 -0.34921384 0.060129572 -0.34921384 
		0.060129572 -0.34921384 0.060129572 -0.34921384 0.060129572 -0.34921384 0.060129572 
		-0.34921384 0.060129572 -0.34921384 0.060129572 -0.34921384 0.060129572 -0.34921384 
		0.060129572 -0.34921384 0.060129572 -0.34921384 0.060129572 -0.34921384 0.060129572 
		-0.34921384 0.060129572 -0.34921384 0.060129572 -0.34921384 0.060129572 -0.34921384 
		0.060129572 -0.34921384 0.060129572 -0.34921384 0.060129572 -0.34921384 0.060129572 
		-0.34921384 0.060129572 -0.34921384 0.060129572 -0.34921384 0.060129572 -0.34921384 
		0.060129572 -0.34921384 0.060129572 -0.34921384 0.060129572 -0.34921384 0.060129572 
		-0.34921384 0.060129572 -0.34921384 0.060129572 -0.34921384 0.060129572 -0.34921384 
		0.060129572 -0.34921384 0.060129572 -0.34921384 0.060129572 -0.34921384 0.060129572 
		-0.34921384 0.060129572 -0.34921384 0.060129572 -0.34921384 0.060129572 -0.34921384 
		0.060129572 -0.34921384 0.060129572 -0.34921384 0.060129572 -0.34921384 0.060129572 
		-0.34921384 0.060129572 -0.34921384 0.060129572 -0.34921384 0.060129572 -0.34921384 
		0.060129572 -0.34921384 0.060129572 -0.34921384 0.060129572 -0.34921384 0.060129572 
		-0.34921384 0.060129572 -0.34921384 0.060129572 -0.34921384 0.060129572 -0.34921384 
		0.060129572 -0.34921384 0.060129572 -0.34921384 0.060129572 -0.34921384 0.060129572 
		-0.34921384 0.060129572 -0.34921384 0.060129572 -0.34921384 0.060129572 -0.34921384 
		0.060129572 -0.34921384 0.060129572 -0.34921384 0.060129572 -0.34921384 0.060129572 
		-0.34921384 0.060129572 -0.34921384 0.060129572 -0.34921384 0.060129572 -0.34921384 
		0.060129572 -0.34921384 0.060129572 -0.34921384 0.060129572 -0.34921384 0.060129572 
		-0.34921384 0.060129572 -0.34921384 0.060129572 -0.34921384 0.060129572 -0.34921384 
		0.060129572 -0.34921384 0.060129572 -0.34921384 0.060129572 -0.34921384 0.060129572 
		-0.34921384 0.060129572 -0.34921384 0.060129572 -0.34921384 0.060129572 -0.34921384 
		0.060129572 -0.34921384 0.060129572 -0.34921384 0.060129572 -0.34921384 0.060129572 
		-0.34921384 0.060129572 -0.34921384 0.060129572 -0.34921384 0.060129572 -0.34921384 
		0.060129572 -0.34921384 0.060129572 -0.34921384 0.060129572 -0.34921384 0.13644786 
		-0.28908429 0.13644783 -0.28908435 0.1364478 -0.28908429 0.13644783 -0.28908429 0.1364478 
		-0.28908435 0.13644783 -0.28908435 0.13644786 -0.28908429 0.1364478 -0.28908429 0.13644786 
		-0.28908435 0.1364478 -0.28908435 0.13644783 -0.28908429 0.13644783 -0.28908435 0.13644783 
		-0.28908429 0.13644783 -0.28908429 0.1364478 -0.28908435 0.13644783 -0.28908435 0.13644786 
		-0.28908429 0.13644783 -0.28908435 0.1364478 -0.28908435 0.1364478 -0.28908435 0.13644783 
		-0.28908435 0.13644782 -0.28908435 0.13644782 -0.28908435 0.13644783 -0.28908435 
		0.1364478 -0.28908435 0.13644782 -0.28908435 0.13644785 -0.28908435 0.13644783 -0.28908429 
		0.1364478 -0.28908429 0.13644782 -0.28908435 0.13644782 -0.28908435 0.13644782 -0.28908429 
		0.13644785 -0.28908437 0.13644783 -0.28908437 0.13644782 -0.28908429 0.1364478 -0.28908435 
		0.13644783 -0.28908435 0.13644785 -0.28908432 0.1364478 -0.28908437 0.13644782 -0.28908437 
		0.13644783 -0.28908432 0.1364478 -0.28908429 0.13644786 -0.28908435;
	setAttr ".uvtk[500:598]" 0.13644786 -0.28908435 0.13644783 -0.28908437 0.13644782 
		-0.28908432 0.13644782 -0.28908432 0.13644782 -0.28908435 0.13644786 -0.28908435 
		0.13644783 -0.28908435 0.13644783 -0.28908437 0.13644782 -0.28908435 0.13644782 -0.28908435 
		0.13644783 -0.28908432 0.13644786 -0.28908435 0.13644782 -0.28908432 0.13644785 -0.28908435 
		0.13644785 -0.28908432 0.1364478 -0.28908437 0.13644783 -0.28908437 0.13644786 -0.28908432 
		0.1364478 -0.28908432 0.13644783 -0.28908437 0.13644783 -0.28908432 0.13644783 -0.28908429 
		0.13644786 -0.28908429 0.1364478 -0.28908437 0.1364478 -0.28908432 0.1364478 -0.28908429 
		0.1364478 -0.28908435 0.13644783 -0.28908429 0.13644786 -0.28908437 0.13644786 -0.28908432 
		0.13644783 -0.28908429 0.1364478 -0.28908435 0.13644783 -0.28908435 0.13644783 -0.28908435 
		0.1364478 -0.28908437 0.13644782 -0.28908432 0.13644783 -0.28908435 0.13644786 -0.28908429 
		0.1364478 -0.28908429 0.13644782 -0.28908435 0.13644783 -0.28908429 0.13644786 -0.28908435 
		0.13644785 -0.28908435 0.13644783 -0.28908435 0.1364478 -0.28908435 0.1364478 -0.28908435 
		0.13644786 -0.28908435 0.13644786 -0.28908435 0.13644783 -0.28908435 0.13644783 -0.28908435 
		0.13644783 -0.28908435 0.13644783 -0.28908435 0.13644783 -0.28908435 0.13644786 -0.28908435 
		0.13644786 -0.28908435 0.13644783 -0.28908435 0.13644786 -0.28908435 0.13644783 -0.28908429 
		0.1364478 -0.28908435 0.13644783 -0.28908435 0.13644783 -0.28908429 0.13644786 -0.28908429 
		0.13644783 -0.28908435 0.13644786 -0.28908437 0.1364478 -0.28908435 0.13644786 -0.28908435 
		0.13644783 -0.28908429 0.1364478 -0.28908435 0.13644783 -0.28908435 0.13644783 -0.28908435 
		0.13644783 -0.28908435 0.13644786 -0.28908435 0.13644783 -0.28908435 0.13644783 -0.28908435 
		0.13644783 -0.28908435 0.1364478 -0.28908435 0.13644786 -0.28908435 0.13644786 -0.28908435 
		0.1364478 -0.28908435 0.13644783 -0.28908435 0.13644783 -0.28908435 0.13644783 -0.28908435 
		0.1364478 -0.28908435 0.13644783 -0.28908435 0.13644783 -0.28908435 0.13644783 -0.28908429 
		0.13644786 -0.28908435 0.13644786 -0.28908435 0.13644783 -0.28908435 0.13644786 -0.28908435 
		0.13644783 -0.28908429 0.13644786 -0.28908435 0.13644783 -0.28908435 0.1364478 -0.28908435 
		0.1364478 -0.28908435 0.1364478 -0.28908435 0.13644783 -0.28908435 0.13644783 -0.28908435 
		0.13644783 -0.28908432;
createNode psdFileTex -n "PSD_lambert_pumpkin_normalCamera";
	setAttr ".ail" yes;
	setAttr ".ftn" -type "string" "C:/Users/jtaibo/Documents/My Dropbox/Bixorama/proyecto_maya//sourceimages/pumpkin_pSphereShape1.psd";
	setAttr ".lsn" -type "string" "lambert_pumpkin.bump";
createNode bump2d -n "bump2d1";
	setAttr ".vc1" -type "float3" 0 0.0001 0 ;
	setAttr ".vc2" -type "float3" 9.9999997e-006 9.9999997e-006 0 ;
createNode place2dTexture -n "place2dTexture4";
createNode file -n "file1";
	setAttr ".ftn" -type "string" "C:/Users/jtaibo/Documents/My Dropbox/Bixorama/proyecto_maya//sourceimages/pumpkin_color.png";
createNode place2dTexture -n "place2dTexture7";
createNode file -n "file2";
	setAttr ".ftn" -type "string" "C:/Users/jtaibo/Documents/My Dropbox/Bixorama/proyecto_maya//sourceimages/pumpkin_transparency.png";
createNode place2dTexture -n "place2dTexture8";
createNode file -n "file3";
	setAttr ".ftn" -type "string" "C:/Users/jtaibo/Documents/My Dropbox/Bixorama/proyecto_maya//sourceimages/pumpkin_incandescence.png";
createNode place2dTexture -n "place2dTexture9";
select -ne :time1;
	setAttr ".o" 1;
select -ne :renderPartition;
	setAttr -s 3 ".st";
select -ne :renderGlobalsList1;
select -ne :defaultShaderList1;
	setAttr -s 3 ".s";
select -ne :postProcessList1;
	setAttr -s 2 ".p";
select -ne :defaultRenderUtilityList1;
	setAttr -s 5 ".u";
select -ne :lightList1;
select -ne :defaultTextureList1;
	setAttr -s 4 ".tx";
select -ne :initialShadingGroup;
	setAttr ".ro" yes;
select -ne :initialParticleSE;
	setAttr ".ro" yes;
select -ne :defaultLightSet;
select -ne :hardwareRenderGlobals;
	setAttr ".ctrs" 256;
	setAttr ".btrs" 512;
select -ne :defaultHardwareRenderGlobals;
	setAttr ".fn" -type "string" "im";
	setAttr ".res" -type "string" "ntsc_4d 646 485 1.333";
connectAttr "polyTweakUV1.out" "pSphereShape1.i";
connectAttr "polyTweakUV1.uvtk[0]" "pSphereShape1.uvst[0].uvtw";
connectAttr ":defaultLightSet.msg" "lightLinker1.lnk[0].llnk";
connectAttr ":initialShadingGroup.msg" "lightLinker1.lnk[0].olnk";
connectAttr ":defaultLightSet.msg" "lightLinker1.lnk[1].llnk";
connectAttr ":initialParticleSE.msg" "lightLinker1.lnk[1].olnk";
connectAttr ":defaultLightSet.msg" "lightLinker1.lnk[2].llnk";
connectAttr "lambert2SG.msg" "lightLinker1.lnk[2].olnk";
connectAttr ":defaultLightSet.msg" "lightLinker1.slnk[0].sllk";
connectAttr ":initialShadingGroup.msg" "lightLinker1.slnk[0].solk";
connectAttr ":defaultLightSet.msg" "lightLinker1.slnk[1].sllk";
connectAttr ":initialParticleSE.msg" "lightLinker1.slnk[1].solk";
connectAttr ":defaultLightSet.msg" "lightLinker1.slnk[2].sllk";
connectAttr "lambert2SG.msg" "lightLinker1.slnk[2].solk";
connectAttr "layerManager.dli[0]" "defaultLayer.id";
connectAttr "renderLayerManager.rlmi[0]" "defaultRenderLayer.rlid";
connectAttr "polyTweak1.out" "polyExtrudeFace1.ip";
connectAttr "pSphereShape1.wm" "polyExtrudeFace1.mp";
connectAttr "polySphere1.out" "polyTweak1.ip";
connectAttr "polyTweak2.out" "polyExtrudeFace2.ip";
connectAttr "pSphereShape1.wm" "polyExtrudeFace2.mp";
connectAttr "polyExtrudeFace1.out" "polyTweak2.ip";
connectAttr "polyTweak3.out" "polyExtrudeFace3.ip";
connectAttr "pSphereShape1.wm" "polyExtrudeFace3.mp";
connectAttr "polyExtrudeFace2.out" "polyTweak3.ip";
connectAttr "polyExtrudeFace3.out" "polyExtrudeFace4.ip";
connectAttr "pSphereShape1.wm" "polyExtrudeFace4.mp";
connectAttr "file1.oc" "lambert_pumpkin.c";
connectAttr "file2.oc" "lambert_pumpkin.it";
connectAttr "file3.oc" "lambert_pumpkin.ic";
connectAttr "bump2d1.o" "lambert_pumpkin.n";
connectAttr "lambert_pumpkin.oc" "lambert2SG.ss";
connectAttr "pSphereShape1.iog" "lambert2SG.dsm" -na;
connectAttr "lambert2SG.msg" "materialInfo1.sg";
connectAttr "lambert_pumpkin.msg" "materialInfo1.m";
connectAttr "file1.msg" "materialInfo1.t" -na;
connectAttr "polyTweak4.out" "polyAutoProj1.ip";
connectAttr "pSphereShape1.wm" "polyAutoProj1.mp";
connectAttr "polyExtrudeFace4.out" "polyTweak4.ip";
connectAttr "polyAutoProj1.out" "polySmoothFace1.ip";
connectAttr "polySmoothFace1.out" "polyTweakUV1.ip";
connectAttr "place2dTexture4.c" "PSD_lambert_pumpkin_normalCamera.c";
connectAttr "place2dTexture4.tf" "PSD_lambert_pumpkin_normalCamera.tf";
connectAttr "place2dTexture4.rf" "PSD_lambert_pumpkin_normalCamera.rf";
connectAttr "place2dTexture4.mu" "PSD_lambert_pumpkin_normalCamera.mu";
connectAttr "place2dTexture4.mv" "PSD_lambert_pumpkin_normalCamera.mv";
connectAttr "place2dTexture4.s" "PSD_lambert_pumpkin_normalCamera.s";
connectAttr "place2dTexture4.wu" "PSD_lambert_pumpkin_normalCamera.wu";
connectAttr "place2dTexture4.wv" "PSD_lambert_pumpkin_normalCamera.wv";
connectAttr "place2dTexture4.re" "PSD_lambert_pumpkin_normalCamera.re";
connectAttr "place2dTexture4.of" "PSD_lambert_pumpkin_normalCamera.of";
connectAttr "place2dTexture4.r" "PSD_lambert_pumpkin_normalCamera.ro";
connectAttr "place2dTexture4.n" "PSD_lambert_pumpkin_normalCamera.n";
connectAttr "place2dTexture4.vt1" "PSD_lambert_pumpkin_normalCamera.vt1";
connectAttr "place2dTexture4.vt2" "PSD_lambert_pumpkin_normalCamera.vt2";
connectAttr "place2dTexture4.vt3" "PSD_lambert_pumpkin_normalCamera.vt3";
connectAttr "place2dTexture4.vc1" "PSD_lambert_pumpkin_normalCamera.vc1";
connectAttr "place2dTexture4.o" "PSD_lambert_pumpkin_normalCamera.uv";
connectAttr "place2dTexture4.ofs" "PSD_lambert_pumpkin_normalCamera.fs";
connectAttr "PSD_lambert_pumpkin_normalCamera.oa" "bump2d1.bv";
connectAttr "place2dTexture7.c" "file1.c";
connectAttr "place2dTexture7.tf" "file1.tf";
connectAttr "place2dTexture7.rf" "file1.rf";
connectAttr "place2dTexture7.mu" "file1.mu";
connectAttr "place2dTexture7.mv" "file1.mv";
connectAttr "place2dTexture7.s" "file1.s";
connectAttr "place2dTexture7.wu" "file1.wu";
connectAttr "place2dTexture7.wv" "file1.wv";
connectAttr "place2dTexture7.re" "file1.re";
connectAttr "place2dTexture7.of" "file1.of";
connectAttr "place2dTexture7.r" "file1.ro";
connectAttr "place2dTexture7.n" "file1.n";
connectAttr "place2dTexture7.vt1" "file1.vt1";
connectAttr "place2dTexture7.vt2" "file1.vt2";
connectAttr "place2dTexture7.vt3" "file1.vt3";
connectAttr "place2dTexture7.vc1" "file1.vc1";
connectAttr "place2dTexture7.o" "file1.uv";
connectAttr "place2dTexture7.ofs" "file1.fs";
connectAttr "place2dTexture8.c" "file2.c";
connectAttr "place2dTexture8.tf" "file2.tf";
connectAttr "place2dTexture8.rf" "file2.rf";
connectAttr "place2dTexture8.mu" "file2.mu";
connectAttr "place2dTexture8.mv" "file2.mv";
connectAttr "place2dTexture8.s" "file2.s";
connectAttr "place2dTexture8.wu" "file2.wu";
connectAttr "place2dTexture8.wv" "file2.wv";
connectAttr "place2dTexture8.re" "file2.re";
connectAttr "place2dTexture8.of" "file2.of";
connectAttr "place2dTexture8.r" "file2.ro";
connectAttr "place2dTexture8.n" "file2.n";
connectAttr "place2dTexture8.vt1" "file2.vt1";
connectAttr "place2dTexture8.vt2" "file2.vt2";
connectAttr "place2dTexture8.vt3" "file2.vt3";
connectAttr "place2dTexture8.vc1" "file2.vc1";
connectAttr "place2dTexture8.o" "file2.uv";
connectAttr "place2dTexture8.ofs" "file2.fs";
connectAttr "place2dTexture9.c" "file3.c";
connectAttr "place2dTexture9.tf" "file3.tf";
connectAttr "place2dTexture9.rf" "file3.rf";
connectAttr "place2dTexture9.mu" "file3.mu";
connectAttr "place2dTexture9.mv" "file3.mv";
connectAttr "place2dTexture9.s" "file3.s";
connectAttr "place2dTexture9.wu" "file3.wu";
connectAttr "place2dTexture9.wv" "file3.wv";
connectAttr "place2dTexture9.re" "file3.re";
connectAttr "place2dTexture9.of" "file3.of";
connectAttr "place2dTexture9.r" "file3.ro";
connectAttr "place2dTexture9.n" "file3.n";
connectAttr "place2dTexture9.vt1" "file3.vt1";
connectAttr "place2dTexture9.vt2" "file3.vt2";
connectAttr "place2dTexture9.vt3" "file3.vt3";
connectAttr "place2dTexture9.vc1" "file3.vc1";
connectAttr "place2dTexture9.o" "file3.uv";
connectAttr "place2dTexture9.ofs" "file3.fs";
connectAttr "lambert2SG.pa" ":renderPartition.st" -na;
connectAttr "lambert_pumpkin.msg" ":defaultShaderList1.s" -na;
connectAttr "bump2d1.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "place2dTexture4.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "place2dTexture7.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "place2dTexture8.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "place2dTexture9.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "lightLinker1.msg" ":lightList1.ln" -na;
connectAttr "pointLightShape1.ltd" ":lightList1.l" -na;
connectAttr "PSD_lambert_pumpkin_normalCamera.msg" ":defaultTextureList1.tx" -na
		;
connectAttr "file1.msg" ":defaultTextureList1.tx" -na;
connectAttr "file2.msg" ":defaultTextureList1.tx" -na;
connectAttr "file3.msg" ":defaultTextureList1.tx" -na;
connectAttr "pointLight1.iog" ":defaultLightSet.dsm" -na;
// End of pumpkin_png.ma
