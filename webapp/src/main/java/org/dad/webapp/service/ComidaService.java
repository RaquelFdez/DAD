package org.dad.webapp.service;

import java.util.List;

import org.dad.webapp.model.Comida;

public interface ComidaService {
	public List<Comida> listaComidas();
	public void anyadirComida(Comida c);
}
