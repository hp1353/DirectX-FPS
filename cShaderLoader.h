#pragma once
class cShaderLoader
{
public:
	cShaderLoader();
	~cShaderLoader();


	LPD3DXEFFECT LoadEffect(char * szFilename);
	

};

