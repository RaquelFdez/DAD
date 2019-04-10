package org.dad.webapp.service;

import java.util.List;

import org.dad.webapp.DAO.ComidaDAO;
import org.dad.webapp.model.Comida;
import org.springframework.transaction.annotation.Transactional;

public class ComidaServiceImpl implements ComidaService {

	private ComidaDAO comidaDAO;
	
	public void setComidaDAO(ComidaDAO comidaDAO){
		this.comidaDAO = comidaDAO;
	}
	
	@Override
	@Transactional
	public List<Comida> listaComidas() {
		
		return this.comidaDAO.listaComidas();
	}

	@Override
	@Transactional
	public void anyadirComida(Comida c) {
		this.comidaDAO.anyadirComida(c);
		
	}

}
