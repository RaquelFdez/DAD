package Dispensador;

import Dispensador.MqttExample;
import io.vertx.core.AbstractVerticle;
import io.vertx.core.Future;

public class Main extends AbstractVerticle {


		// TODO Auto-generated method stub
		public void start(Future<Void> startFuture) {
			vertx.deployVerticle(new HttpServer());
			vertx.deployVerticle(new RestServerDataBase());
			vertx.deployVerticle(new MqttExample());
			
		}
	}

