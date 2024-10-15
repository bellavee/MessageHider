#pragma once

#include <Windows.h>

class Button
{
public:

    // - name : le nom du bouton (texte affich�)
	// - color : la couleur du bouton (d�finie par un code COLORREF)
	// - x, y : la position du coin sup�rieur gauche du bouton dans la fen�tre parent
	// - width, height : les dimensions du bouton
	// - id : identifiant du bouton, utilis� pour la gestion des �v�nements
	// - hInstance : handle de l'instance de l'application
	// - parent : handle de la fen�tre parente qui contiendra le bouton
	Button(const wchar_t* name, COLORREF color, int x, int y, int width, int height, HMENU id, HINSTANCE hInstance, HWND parent);
	virtual ~Button();

	void Create();
	virtual void OnClick();

	HWND GetHandle() const;

protected:

	HWND hWnd;

	const wchar_t* m_name;
	COLORREF m_color;
	int m_x, m_y, m_width, m_height;
	HMENU m_id;
	HINSTANCE m_hInstance;
	HWND m_parent;
};

