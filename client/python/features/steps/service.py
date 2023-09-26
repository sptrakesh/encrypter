from behave import given, then, step
from behave.api.async_step import async_run_until_complete
from behave.runner import Context
from hamcrest import assert_that, equal_to, not_

from client import Client as _Client


@given("The encrypter service running on {host} on port {port:d}")
def step_impl(context: Context, host: str, port: int):
    context.host = host
    context.port = port
    context.value = "0ENKfo4NkIAHVBoNUtUseShHK/6JQ11YGALUSA/RLcICLQaTyxDJcldEsxxM0h+AWdkXAXNY6xOhCk+TkobTayscjTPqu6bzKIxnD1hso8Hz/ZWsh4SlSxHLhHAZRg6VfBZlITexMDdMOks2eauDdND6ft5kT+7uJFpq5EfnuHE="


@then("Encrypt a string")
@async_run_until_complete
async def step_impl(context: Context):
    assert_that(hasattr(context, "host"), equal_to(True), "Encrypter service host not set")
    assert_that(hasattr(context, "port"), equal_to(True), "Encrypter service port not set")
    assert_that(hasattr(context, "value"), equal_to(True), "String to encrypt not set")

    async with _Client(host=context.host, port=context.port) as client:
        value = await client.encrypt(context.value)
        assert_that(len(value), not_(equal_to(0)), "Encrypted value of zero length")
        context.encrypted = value


@step("Decrypt the encrypted string")
@async_run_until_complete
async def step_impl(context: Context):
    assert_that(hasattr(context, "host"), equal_to(True), "Encrypter service host not set")
    assert_that(hasattr(context, "port"), equal_to(True), "Encrypter service port not set")
    assert_that(hasattr(context, "value"), equal_to(True), "String to encrypt not set")
    assert_that(hasattr(context, "encrypted"), equal_to(True), "Encrypted string not set")

    async with _Client(host=context.host, port=context.port) as client:
        value = await client.decrypt(context.encrypted)
        assert_that(value, equal_to(context.value), "Decrypted value not same as original")
