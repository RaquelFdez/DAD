package Dispensador;

import io.vertx.core.AbstractVerticle;
import io.vertx.core.Future;
import io.vertx.core.json.JsonArray;
import io.vertx.core.json.JsonObject;
import io.vertx.ext.asyncsql.AsyncSQLClient;
import io.vertx.ext.asyncsql.MySQLClient;
import io.vertx.ext.sql.SQLConnection;
import io.vertx.ext.sql.UpdateResult;
import io.vertx.ext.web.Router;
import io.vertx.ext.web.RoutingContext;
import io.vertx.ext.web.handler.BodyHandler;

public class RestServerDataBase extends AbstractVerticle {
	
	private AsyncSQLClient mySQLClient;
	
	public void start(Future<Void> startFuture) {
		JsonObject config = new JsonObject()
				.put("host", "localhost")
				.put("username", "root")
				.put("password", "mysqlrootpasswordhere")
				.put("database", "Dispensador")
				.put("port", 3306);
		mySQLClient = 
				MySQLClient.createShared(vertx, config);


	Router router = Router.router(vertx);
	
	vertx.createHttpServer().requestHandler(router).
		listen(8090, result -> {
			if (result.succeeded()) {
				System.out.println("Servidor database desplegado");
			}else {
				System.out.println("Error de despliegue");
			}
		});
	router.route().handler(BodyHandler.create());
	router.get("/comidas").handler(this::handleAllSensors);
	router.put("/comidas").handler(this::handleProductProperty);
}
	
	
	private void handleAllSensors(RoutingContext routingConext) {
		mySQLClient.getConnection(connection -> {
			if (connection.succeeded()) {
				connection.result().query("SELECT * FROM Dispensador.Comida" , result -> {
					if (result.succeeded()) {
						String jsonResult = result.result().toJson().encodePrettily();
						routingConext.response().end(new JsonArray(result.result().getRows()).encodePrettily());
						System.out.println(jsonResult);
					}else {
						System.out.println(result.cause().getMessage());
						routingConext.response().setStatusCode(400).end();
					}
					connection.result().close();
				});
			}else {
				connection.result().close();
				System.out.println(connection.cause().getMessage());
				routingConext.response().setStatusCode(400).end();
			}
		});
	}
	
	private void handleProductProperty(RoutingContext routingContext) {
		mySQLClient.getConnection(connection -> {
			if(connection.succeeded()) {
				System.out.println(routingContext.getBodyAsJson().encodePrettily());
				connection.result().update("INSERT INTO Dispensador.Comida (nombre, horas, pesosPlato, pesosDeposito, pesosAviso) VALUES ('" + routingContext.getBodyAsJson().getString("nombre") + "',1800,200,10000,2000)", res->{
					if(res.succeeded()) {
						UpdateResult result = res.result();
					    System.out.println("Updated no. of rows: " + result.getUpdated());
					    System.out.println("Generated keys: " + result.getKeys());
					    routingContext.response().setStatusCode(201).end(result.getKeys().encodePrettily());
					}else {
						System.out.println(res.cause().getMessage());
						routingContext.response().setStatusCode(400).end();
					}
					connection.result().close();
				});
			}else {
				connection.result().close();
				System.out.println(connection.cause().getMessage());
				routingContext.response().setStatusCode(400).end();
			}
		});
	}
	
	
}	