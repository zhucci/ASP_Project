#include "OCCT_Headers.h"
namespace asp{
	class Color{
	public:
		static Quantity_NameOfColor name(int id);
	private:
		static int ColorAmount;
		static Quantity_NameOfColor colorVar[];
			
	};
};
