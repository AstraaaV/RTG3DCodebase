#include "Texture.h"
#include "TextureLoader.h"
#include "stringHelp.h"

Texture::Texture(ifstream& _file)
{
	string type;
	string fileName;
	StringHelp::String(_file, "TYPE", type);
	StringHelp::String(_file, "NAME", m_name);
	StringHelp::String(_file, "FILE", fileName);
	FREE_IMAGE_FORMAT format = FIF_UNKNOWN;

	if (type == "FIF_BMP")
	{
		format = FIF_BMP;
	}
	else if (type == "FIF_ICO")
	{
		format = FIF_ICO;
	}
	else if (type == "FIF_JPEG")
	{
		format = FIF_JPEG;
	}
	else if (type == "FIF_JNG")
	{
		format = FIF_JNG;
	}
	else if (type == "FIF_KOALA")
	{
		format = FIF_KOALA;
	}
	else if (type == "FIF_LBM")
	{
		format = FIF_LBM;
	}
	else if (type == "FIF_IFF") //note different name for same format
	{
		format = FIF_LBM;
	}
	else if (type == "FIF_MNG")
	{
		format = FIF_MNG;
	}
	else if (type == "FIF_PBM")
	{
		format = FIF_PBM;
	}
	else if (type == "FIF_PBMRAW")
	{
		format = FIF_PBMRAW;
	}
	else if (type == "FIF_PCD")
	{
		format = FIF_PCD;
	}
	else if (type == "FIF_PCX")
	{
		format = FIF_PCX;
	}
	else if (type == "FIF_PGM")
	{
		format = FIF_PGM;
	}
	else if (type == "FIF_PGMRAW")
	{
		format = FIF_PGMRAW;
	}
	else if (type == "FIF_PNG")
	{
		format = FIF_PNG;
	}
	else if (type == "FIF_PPM")
	{
		format = FIF_PPM;
	}
	else if (type == "FIF_PPMRAW")
	{
		format = FIF_PPMRAW;
	}
	else if (type == "FIF_RAS")
	{
		format = FIF_RAS;
	}
	else if (type == "FIF_TARGA") //this for TGA
	{
		format = FIF_TARGA;
	}
	else if (type == "FIF_TIFF")
	{
		format = FIF_TIFF;
	}
	else if (type == "FIF_WBMP")
	{
		format = FIF_WBMP;
	}
	else if (type == "FIF_PSD")
	{
		format = FIF_PSD;
	}
	else if (type == "FIF_CUT")
	{
		format = FIF_CUT;
	}
	else if (type == "FIF_XBM")
	{
		format = FIF_XBM;
	}
	else if (type == "FIF_XPM")
	{
		format = FIF_XPM;
	}
	else if (type == "FIF_DDS")
	{
		format = FIF_DDS;
	}
	else if (type == "FIF_GIF")
	{
		format = FIF_GIF;
	}
	else if (type == "FIF_HDR")
	{
		format = FIF_HDR;
	}
	else if (type == "FIF_FAXG3")
	{
		format = FIF_FAXG3;
	}
	else if (type == "FIF_SGI")
	{
		format = FIF_SGI;
	}
	else if (type == "FIF_EXR")
	{
		format = FIF_EXR;
	}
	else if (type == "FIF_J2K")
	{
		format = FIF_J2K;
	}
	else if (type == "FIF_JP2")
	{
		format = FIF_JP2;
	}
	else if (type == "FIF_PFM")
	{
		format = FIF_PFM;
	}
	else if (type == "FIF_PICT")
	{
		format = FIF_PICT;
	}
	else if (type == "FIF_RAW")
	{
		format = FIF_RAW;
	}
	else if (type == "FIF_WEBP")
	{
		format = FIF_WEBP;
	}
	else if (type == "FIF_JXR")
	{
		format = FIF_JXR;
	}
	else
	{
		cout << "Unknown Texture type : " << type << endl;
		assert(0);
	}


	m_texID = loadTexture(fileName, format);
}

Texture::~Texture()
{
	if (m_texID)
	{
		glDeleteTextures(1, &m_texID);
		m_texID = 0;
	}
}
