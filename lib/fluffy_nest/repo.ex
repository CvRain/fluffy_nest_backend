defmodule FluffyNest.Repo do
  use Ecto.Repo,
    otp_app: :fluffy_nest,
    adapter: Ecto.Adapters.Postgres
end
