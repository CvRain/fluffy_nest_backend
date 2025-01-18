defmodule FluffyNest.Repo.Migrations.CreateUsers do
  use Ecto.Migration

  def change do
    create table(:users) do
      add :name, :string
      add :password, :string
      add :email, :string
      add :icon, :string
      add :sign, :string

      timestamps(type: :utc_datetime)
    end
  end
end