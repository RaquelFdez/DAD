package org.dad.webapp.DAO;

import java.util.List;

import org.dad.webapp.model.Comida;

public interface ComidaDAO {
	public List<Comida> listaComidas();
	public void anyadirComida(Comida c);
}
