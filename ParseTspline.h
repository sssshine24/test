#pragma once
class CSceneGraph3d;
class CParseTspline
{
public:
	void Free(void);
	int Run(char *filename,CSceneGraph3d *pSceneGraph);
	CParseTspline(void);
	~CParseTspline(void);
	CString m_FileName;
};
