#pragma once

#include <Windows.h>

class Button
{
public:

    // - name : le nom du bouton (texte affiché)
	// - color : la couleur du bouton (définie par un code COLORREF)
	// - x, y : la position du coin supérieur gauche du bouton dans la fenêtre parent
	// - width, height : les dimensions du bouton
	// - id : identifiant du bouton, utilisé pour la gestion des événements
	// - hInstance : handle de l'instance de l'application
	// - parent : handle de la fenêtre parente qui contiendra le bouton
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

