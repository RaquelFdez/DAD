package org.dad.webapp.model;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.Table;

@Entity
@Table(name="Comida")
public class Comida {

	@Id
	@Column(name="idComida")
	@GeneratedValue(strategy=GenerationType.IDENTITY)
	private Integer idComida;
	
	@Column(name="nombre")
	private String nombre;
	
	@Column(name="horas")
	private Integer horas;
	
	@Column(name="pesoPlato")
	private Integer pesoPlato;
	
	@Column(name="pesoDeposito")
	private Integer pesoDeposito;
	
	@Column(name="pesoAviso")
	private Integer pesoAviso;

	public Integer getIdComida() {
		return idComida;
	}

	public void setIdComida(Integer idComida) {
		this.idComida = idComida;
	}

	public String getNombre() {
		return nombre;
	}

	public void setNombre(String nombre) {
		this.nombre = nombre;
	}

	public Integer getHoras() {
		return horas;
	}

	public void setHoras(Integer horas) {
		this.horas = horas;
	}

	public Integer getPesoPlato() {
		return pesoPlato;
	}

	public void setPesoPlato(Integer pesoPlato) {
		this.pesoPlato = pesoPlato;
	}

	public Integer getPesoDeposito() {
		return pesoDeposito;
	}

	public void setPesoDeposito(Integer pesoDeposito) {
		this.pesoDeposito = pesoDeposito;
	}

	public Integer getPesoAviso() {
		return pesoAviso;
	}

	public void setPesoAviso(Integer pesoAviso) {
		this.pesoAviso = pesoAviso;
	}
	
	
	
	
}
