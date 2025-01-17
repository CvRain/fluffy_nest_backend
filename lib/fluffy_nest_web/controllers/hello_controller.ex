defmodule FluffyNestWeb.HelloController do
  use FluffyNestWeb, :controller

  def index(conn, _params) do
      message = %{
        "message" => "Hello World!",
        "code" => 200,
        "current_date" => DateTime.utc_now() |> DateTime.to_iso8601(),
      }
      json(conn, message)
  end

  def current(conn, _params) do
    now = DateTime.utc_now()
    message = %{
      "message" => "Hello World!",
      "current_date" => now |> DateTime.to_iso8601(),
      "timestamp" => now |> DateTime.to_unix()
    }
    json(conn, message)
  end
end
