#pragma once

//elements for constructing value for which render passes an object should be drawn in
enum RenderPass
{
	RP_NONE = 0,
	RP_BACKGROUND = 1 << 0,
	RP_OPAQUE = 1 << 1,
	RP_TRANSPARENT = 1 << 2,
};