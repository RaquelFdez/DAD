package Dispensador;

import io.vertx.core.AbstractVerticle;
import io.vertx.core.Future;

public class Main extends AbstractVerticle {


		// TODO Auto-generated method stub
		public void start(Future<Void> startFuture) {
			vertx.deployVerticle(new HttpServer());
			vertx.deployVerticle(new RestServerDataBase());
			
		}
	}

