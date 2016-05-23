#include "GoodColor.h"
using namespace asp;
int asp::Color::ColorAmount = 128;
Quantity_NameOfColor Color::colorVar[] = {
	Quantity_NOC_ROSYBROWN,
	Quantity_NOC_ROYALBLUE,
	Quantity_NOC_SANDYBROWN,
	Quantity_NOC_BEET,
	Quantity_NOC_TEAL,
	Quantity_NOC_SIENNA,

	Quantity_NOC_SKYBLUE3,

	Quantity_NOC_SLATEBLUE,

	Quantity_NOC_SLATEGRAY,

	Quantity_NOC_SPRINGGREEN,

	Quantity_NOC_STEELBLUE,

	Quantity_NOC_BURLYWOOD,
	Quantity_NOC_BLUE4,

	Quantity_NOC_ALICEBLUE,
	Quantity_NOC_AZURE2,

	Quantity_NOC_AZURE4,
	Quantity_NOC_BEIGE,
	Quantity_NOC_BISQUE,
	Quantity_NOC_BISQUE2,

	Quantity_NOC_BISQUE4,
	Quantity_NOC_BLANCHEDALMOND,
	Quantity_NOC_BLUE1,

	Quantity_NOC_BLUEVIOLET,
	Quantity_NOC_BROWN,

	Quantity_NOC_BROWN3,
	Quantity_NOC_BROWN4,


	Quantity_NOC_BURLYWOOD3,
	Quantity_NOC_CADETBLUE,

	Quantity_NOC_CHARTREUSE,



	Quantity_NOC_CORAL,

	Quantity_NOC_CORNFLOWERBLUE,

	Quantity_NOC_CYAN1,

	Quantity_NOC_CYAN4,
	Quantity_NOC_DARKGOLDENROD,
	Quantity_NOC_DARKGOLDENROD4,
	Quantity_NOC_DARKGREEN,
	Quantity_NOC_DARKKHAKI,
	Quantity_NOC_DARKOLIVEGREEN,

	Quantity_NOC_DARKOLIVEGREEN4,
	Quantity_NOC_DARKORANGE,

	Quantity_NOC_DARKORANGE4,
	Quantity_NOC_DARKORCHID,
	Quantity_NOC_DARKORCHID4,
	Quantity_NOC_DARKSALMON,


	Quantity_NOC_DARKSEAGREEN2,

	Quantity_NOC_DARKSLATEBLUE,

	Quantity_NOC_DARKSLATEGRAY,
	Quantity_NOC_DARKTURQUOISE,
	Quantity_NOC_DARKVIOLET,
	Quantity_NOC_DEEPPINK,

	Quantity_NOC_DEEPSKYBLUE2,

	Quantity_NOC_DODGERBLUE2,

	Quantity_NOC_FIREBRICK,

	Quantity_NOC_FLORALWHITE,
	Quantity_NOC_FORESTGREEN,
	Quantity_NOC_GAINSBORO,
	Quantity_NOC_GHOSTWHITE,
	Quantity_NOC_GOLD,

	Quantity_NOC_GOLDENROD,

	Quantity_NOC_GRAY,

	Quantity_NOC_GRAY26,

	Quantity_NOC_GRAY5,

	Quantity_NOC_GRAY7,

	Quantity_NOC_GREEN2,

	Quantity_NOC_GREENYELLOW,
	Quantity_NOC_GRAY97,


	Quantity_NOC_HONEYDEW2,

	Quantity_NOC_HOTPINK,

	Quantity_NOC_INDIANRED,
	Quantity_NOC_INDIANRED1,

	Quantity_NOC_IVORY,

	Quantity_NOC_KHAKI,

	Quantity_NOC_LAVENDER,

	Quantity_NOC_LAWNGREEN,

	Quantity_NOC_LIGHTBLUE,

	Quantity_NOC_LIGHTBLUE4,
	Quantity_NOC_LIGHTCORAL,

	Quantity_NOC_LIGHTGOLDENROD,

	Quantity_NOC_LIGHTGOLDENRODYELLOW,
	Quantity_NOC_LIGHTGRAY,

	Quantity_NOC_LIGHTSALMON2,
	Quantity_NOC_LIGHTSEAGREEN,
	Quantity_NOC_LIGHTSKYBLUE,

	Quantity_NOC_LIGHTSLATEBLUE,
	Quantity_NOC_LIGHTSLATEGRAY,
	Quantity_NOC_LIGHTSTEELBLUE,

	Quantity_NOC_LIGHTYELLOW,

	Quantity_NOC_LIMEGREEN,
	Quantity_NOC_LINEN,
	Quantity_NOC_MAGENTA1,

	Quantity_NOC_MAROON,

	Quantity_NOC_MEDIUMAQUAMARINE,
	Quantity_NOC_MEDIUMORCHID,

	Quantity_NOC_MEDIUMPURPLE,

	Quantity_NOC_MEDIUMSEAGREEN,
	Quantity_NOC_MEDIUMSLATEBLUE,
	Quantity_NOC_MEDIUMSPRINGGREEN,
	Quantity_NOC_MEDIUMTURQUOISE,
	Quantity_NOC_MEDIUMVIOLETRED,
	Quantity_NOC_MIDNIGHTBLUE,
	Quantity_NOC_MINTCREAM,
	Quantity_NOC_MISTYROSE,

	Quantity_NOC_MOCCASIN,

	Quantity_NOC_OLDLACE,
	Quantity_NOC_OLIVEDRAB,

	Quantity_NOC_ORANGE,

	Quantity_NOC_ORANGERED,

	Quantity_NOC_ORCHID,

	Quantity_NOC_PALEGOLDENROD,

	Quantity_NOC_PALETURQUOISE,

	Quantity_NOC_PALEVIOLETRED,

	Quantity_NOC_PAPAYAWHIP,

	Quantity_NOC_PERU,

	Quantity_NOC_PINK4,

	Quantity_NOC_POWDERBLUE,
	Quantity_NOC_PURPLE,


	Quantity_NOC_TAN,

	Quantity_NOC_THISTLE,

	Quantity_NOC_TOMATO,

	Quantity_NOC_TURQUOISE,

	Quantity_NOC_VIOLET,

	Quantity_NOC_WHEAT,
	Quantity_NOC_WHEAT1,


	Quantity_NOC_YELLOW3,
};
Quantity_NameOfColor Color::name(int id){
	int var = (int)(id/(ColorAmount-1));
	var = id - var*ColorAmount;
	return colorVar[var];
}