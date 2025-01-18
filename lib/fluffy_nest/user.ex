defmodule FluffyNest.User do
  use Ecto.Schema
  import Ecto.Changeset

  schema "users" do
    field :name, :string
    field :sign, :string
    field :password, :string
    field :email, :string
    field :icon, :string

    timestamps(type: :utc_datetime)
  end

  @doc false
  def changeset(user, attrs) do
    user
    |> cast(attrs, [:name, :password, :email, :icon, :sign])
    |> validate_required([:name, :password, :email, :icon, :sign])
  end
end
