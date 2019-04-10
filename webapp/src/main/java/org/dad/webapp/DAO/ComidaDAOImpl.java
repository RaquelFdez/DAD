package org.dad.webapp.DAO;

import org.slf4j.LoggerFactory;

import java.util.List;

import org.dad.webapp.model.Comida;
import org.hibernate.Session;
import org.hibernate.SessionFactory;
import org.slf4j.Logger;
import org.springframework.stereotype.Repository;

@Repository
public class ComidaDAOImpl implements ComidaDAO {
	
	private static final Logger logger = LoggerFactory.getLogger(ComidaDAOImpl.class);
	
	private SessionFactory sessionFactory;
	
	public void setSessionFactory(SessionFactory sf){
		this.sessionFactory = sf;
	}

	@Override
	public List<Comida> listaComidas() {
		Session session = this.sessionFactory.getCurrentSession();
		List<Comida> list = session.createQuery("from Comida").list();
		for(Comida c : list){
			logger.info("Lista de comidas registradas::"+c);
		}
		return list;
	}

	@Override
	public void anyadirComida(Comida c) {
		Session session = this.sessionFactory.getCurrentSession();
		session.persist(c);
		logger.info("Comida añadida");
		
	}
	
	

	
}
