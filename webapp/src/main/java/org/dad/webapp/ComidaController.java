package org.dad.webapp;

import org.dad.webapp.model.Comida;
import org.dad.webapp.service.ComidaService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Qualifier;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.ModelAttribute;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;

@Controller
public class ComidaController {
	
	private ComidaService comidaService;
	
	@Autowired(required=true)
	@Qualifier(value="personService")
	public void setComidaService(ComidaService cs){
		this.comidaService = cs;
	}
	
	@RequestMapping(value = "/comidas", method = RequestMethod.GET)
	public String listPersons(Model model) {
		model.addAttribute("person", new Comida());
		model.addAttribute("listPersons", this.comidaService.listaComidas());
		return "person";
	}
	
	@RequestMapping(value= "/person/add", method = RequestMethod.POST)
	public String addPerson(@ModelAttribute("person") Comida c){
		
		this.comidaService.anyadirComida(c);
		
		return "redirect:/persons";
		
	}
	
}
