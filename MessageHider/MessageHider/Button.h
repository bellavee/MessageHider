#pragma once

#include <Windows.h>
#include "AppManager.h"
#include "LSB.h"

enum class ButtonType
{
	EncodePage,
	DecodePage,
	Load,
	Download,
	Theme,
	EncodeAction,
	DecodeAction
};

class Button
{
public:
	Button(
		ButtonType type,
		COLORREF color,			// la couleur du bouton (d�finie par un code COLORREF)
		int x, int y,			// la position du coin sup�rieur gauche du bouton dans la fen�tre parent
		int width, int height,	// les dimensions du bouton
		HINSTANCE hInstance,	// handle de l'instance de l'application
		HWND parent			// handle de la fen�tre parente qui contiendra le bouton
	);

	virtual ~Button();

	void Create();
	virtual void OnClick();

	//HWND GetHandle() const;
	HMENU GetId() const { return m_id; }

private:

	HWND hWnd;

	const wchar_t* m_name;
	COLORREF m_color;
	int m_x, m_y, m_width, m_height;
	HMENU m_id;
	HINSTANCE m_hInstance;
	HWND m_parent; // parent
	ButtonType m_type;
};